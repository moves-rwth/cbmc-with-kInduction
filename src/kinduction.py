"""
Main entry file. Handles the general structure of the verification process, including process execution and result
interpretation.
"""
import argparse
import fcntl
import io
import os
import re
import shutil
import signal
import subprocess
import tempfile
import time

import psutil
import yaml

from argparse import Namespace

from pycparserext.ext_c_parser import GnuCParser
from pycparserext.ext_c_generator import GnuCGenerator
from canalyzer import *
from ctransformer import *

from modules.variablemoving.variablemoving import variable_analysis_from_file
from modules.slicing.slicing import static_slicing_from_file

# Default configuration. Real configuration is read from the user's configuration file.
from witnessgml import extend_from_cbmc_to_cpa_format

VERIFIER_IS_INCREMENTAL       = False
VERIFIER_BASE_CALL            = "cbmc.sh --unwindset main.X:KINCREMENT --no-unwinding-assertions".split()
VERIFIER_INDUCTION_CALL       = "cbmc.sh --unwindset main.X:KINCREMENT --no-unwinding-assertions".split()
VERIFIER_KINCREMENT_STRING    = "KINCREMENT"
VERIFIER_WITNESS_FILENAME_STRING    = "FILENAME"
VERIFIER_WITNESS_GEN_ARGUMENT    = "--graphml-witness FILENAME"
VERIFIER_WITNESS_BASE_FILENAME = "base"
VERIFIER_WITNESS_INDUCTION_FILENAME = "induction"
VERIFIER_FALSE_REGEX          = "VERIFICATION FAILED"
VERIFIER_TRUE_REGEX           = "VERIFICATION SUCCESSFUL"
VERIFIER_K_REGEX              = "VERIFICATION FAILED|VERIFICATION SUCCESSFUL"
VERIFIER_SMT_TIME_REGEX_START = "Runtime decision procedure: "
VERIFIER_SMT_TIME_REGEX_END   = "s"
VERIFIER_ASSUME_FUNCTION_NAME = "__VERIFIER_assume"
MAIN_FUNCTION_NAME            = "main"
POLL_INTERVAL                 = 2

def signal_handler(sig, frame):
	"""
	Captures the shutdown signals and cleans up all child processes of this process.
	"""
	parent = psutil.Process(os.getpid())
	for child in parent.children(recursive=True):
		child.kill()
	sys.exit(0)

def prepare_base_step(input_file: str):
	"""
	Prepares the input C file for the execution of the base step. As for now, nothing needs to be adapted, so the
	content is just copied to a temporary working file.
	:param input_file: The path of the input C file to prepare.
	:return: The location of the prepared C file for the base step.
	:rtype: str
	"""
	output_file = tempfile.NamedTemporaryFile(delete=False, suffix=".c")
	shutil.copy(input_file, output_file.name)
	return output_file.name

def prepare_induction_step(input_file: str, original_input_file: str=None):
	"""
	Prepares the input C file for the execution of the induction step. It parses the code, havocs the main loop
	variables and adds the property assumption to the beginning of the loop body. When finished, the C code is written
	to a temporary working file which is then returned.
	:param input_file: The input C file location to prepare.
	:param input_file: The input C file location of the original input code, in case analyses were previously applied.
	:return: The location of the prepared C file for the induction step.
	:rtype: str
	"""
	parser = GnuCParser()
	with open(input_file) as file:
		ast = parser.parse(file.read())
	analyzer    = CAnalyzer(ast)
	transformer = CTransformer(ast)
	# De-anonymizes aggregates as a first step, as this is a requirement for later analysis.
	# transformer.deanonymize_aggregates()
	# Identifies main components of the code.
	try:
		main_function = analyzer.identify_function(MAIN_FUNCTION_NAME)
		main_loop     = analyzer.identify_main_loop()
		declarations  = analyzer.identify_declarations_of_modified_variables(main_loop.stmt, main_function)
		if original_input_file and input_file != original_input_file:
			with open(original_input_file) as original_file:
				# In case we sliced the input, we want to re-add the original property first, as Frama-C scrambles the
				# if statement in such a way that it becomes unrecognizable for our property identification process.
				transformer.add_property(CAnalyzer(parser.parse(original_file.read())).identify_property(), main_loop)
		property = analyzer.identify_property()
	except (NoSuchFunctionException,
			NoMainLoopException,
			MultipleMainLoopsException,
			UnidentifiableVariableTypeException) as err:
		print(err)
		sys.exit(1)
	# Creates new code and functions to be added.
	try:
		svcomp_havoc_functions, havoc_block = transformer.create_havoc_block(declarations)
	except NonSvCompTypeException as err:
		print(err)
		sys.exit(1)
	for svcomp_havoc_function in svcomp_havoc_functions:
		transformer.insert(transformer.create_svcomp_function_declaration(svcomp_havoc_function), before=ast.ext[0])
	conjunct_property = transformer.join_expression_list("&&", property)
	negated_property  = transformer.add_to_expression(conjunct_property, "!")
	assume_property   = transformer.create_function_call(VERIFIER_ASSUME_FUNCTION_NAME, negated_property)
	# Adds some code to emulate incremental BMC: Only check property if we are in the k-th main loop iteration.
	if not VERIFIER_IS_INCREMENTAL:
		k_initialization = transformer.from_code("const unsigned int k = 1;", parser).block_items[0]
		i_initialization = transformer.from_code("unsigned int i = 0;", parser).block_items[0]
		i_increment      = transformer.from_code("i++;", parser).block_items[0]
		k_property       = transformer.add_to_expression(property.exprs[0],
														 "&&",
														 c_ast.ExprList(transformer.from_code("(i == k)").block_items))
		transformer.replace_property(k_property)
		transformer.insert(k_initialization, before=main_loop)
		transformer.insert(i_initialization, before=main_loop)
		transformer.insert(i_increment, before=main_loop.stmt.block_items[0])
	# Inserts new code pieces.
	transformer.insert(havoc_block, before=main_loop)
	transformer.insert(assume_property, before=main_loop.stmt.block_items[0])
	# Writes the transformed code to a temporary file.
	output_file = tempfile.NamedTemporaryFile(delete=False, suffix=".c")
	output_file.write(bytes(GnuCGenerator().visit(ast), "utf-8"))
	return output_file.name

def interprete_base_step_output(output: str):
	"""
	Interprets the given verifier output to one of the two possible outcomes of a base step verification task:
	False and Unknown.
	:param output: The output of the verifier.
	:return: False or None.
	"""
	if re.compile(VERIFIER_FALSE_REGEX).search(output) is not None:
		return False
	else:
		return None

def interprete_induction_step_output(output: str):
	"""
	Interprets the given verifier output to one of the two possible outcomes of a induction step verification task:
	True and Unknown.
	:param output: The output of the verifier.
	:return: True or None.
	"""
	if re.compile(VERIFIER_TRUE_REGEX).search(output) is not None:
		return True
	else:
		return None

def identify_k(output: str) -> int:
	"""
	Identifies the current iteration number given the verifier output.
	:param output: A (partial) output of a verifier.
	:return: The iteration number.
	:rtype: int
	"""
	# Returns the number of occurrences of the verifier "k" regex plus one (as we start with k=1).
	return len(re.compile(VERIFIER_K_REGEX).findall(output)) + 1

def identify_smt_time(output: str, only_last:bool=False):
	"""
	Identifies the time that was spent on SMT-solving.
	:param output: A (partial) output of a verifier.
	:param only_last: If set, only the last SMT-time is returned if the string contains multiple times.
	:return: The SMT time.
	:rtype: float
	"""
	time = 0
	matches = re.findall(VERIFIER_SMT_TIME_REGEX_START + "(.*?)" + VERIFIER_SMT_TIME_REGEX_END, output)
	if len(matches) > 0 and only_last:
		matches = [matches[-1]]
	for match in matches:
		try:
			time += float(str(match))
		except Exception as e:
			print("Can not identify the SMT-time " + str(match) + " as a float value!")
			print(e)
	return time

def is_timeout(timelimit: int) -> bool:
	"""
	Checks if the tool has run into a timeout, given the timelimit that was set by the user.
	:param timelimit: The timelimit in seconds. Can be None.
	:return: True or False.
	:rtype: True or False.
	"""
	if timelimit:
		# Collects system and user time for children and self.
		sum_time = sum(psutil.Process(os.getpid()).cpu_times()) +\
				   sum([sum(c.cpu_times()) for c in psutil.Process(os.getpid()).children(recursive=True)])
		if sum_time >= timelimit:
			print("Timeout after " + "%.2f" % round(sum_time, 2) + " seconds.")
			return True
		else:
			return False
	else:
		return False

def run_kinduction_incremental_bmc(file_base: str, file_induction: str, timelimit: int=None, print_smt_time:bool=False):
	"""
	Runs the k-Induction algorithm on the given files. Starts two processes, one for the base step, one for the
	induction step. Returns the answer to the verification task as soon as one of the processes stop. Note that this
	function call may run indefinitely. This function shall only be used if the underlying verifier supports an
	incremental BMC setting.
	:param file_base: The location of the file to run the base step on.
	:param file_induction: The location of the file to run the inductive step on.
	:param timelimit: An optional limit on the CPU-time, in seconds.
	:param print_smt_time: Whether to print out the time that was spent on SMT-solving.
	:return: Either True, False or None (in case no definite answer could be given).
	"""
	# Starts both processes.
	base_out_file      = tempfile.TemporaryFile()
	induction_out_file = tempfile.TemporaryFile()
	base_process       = subprocess.Popen(VERIFIER_BASE_CALL + [file_base], stdout=base_out_file)
	induction_process  = subprocess.Popen(VERIFIER_INDUCTION_CALL + [file_induction], stdout=induction_out_file)
	# Sets stdout to non-blocking IO, so we are able to fetch intermediate data without blocking if no data was read.
	fl = fcntl.fcntl(base_out_file, fcntl.F_GETFL)
	fcntl.fcntl(base_out_file, fcntl.F_SETFL, fl | os.O_NONBLOCK)
	fl = fcntl.fcntl(induction_out_file, fcntl.F_GETFL)
	fcntl.fcntl(induction_out_file, fcntl.F_SETFL, fl | os.O_NONBLOCK)
	base_step_output      = ""
	induction_step_output = ""
	base_step_k           = 0
	induction_step_k      = 0
	# Busy waiting until one of the processes finishes. Already fetching intermediate output here for some processing.
	while True:
		# Checks for a possible timeout.
		if is_timeout(timelimit): break
		# Identifies new output.
		base_out_file.seek(0)
		induction_out_file.seek(0)
		base_step_output = base_out_file.read().decode("utf-8")
		induction_step_output = induction_out_file.read().decode("utf-8")
		base_out_file.seek(0, io.SEEK_END)
		induction_out_file.seek(0, io.SEEK_END)
		# Checks if one of the processes has reached a new k, and prints it.
		old_base_step_k      = base_step_k
		old_induction_step_k = induction_step_k
		base_step_k          = identify_k(base_step_output)
		induction_step_k     = identify_k(induction_step_output)
		# Quits if the base process has found a counterexample.
		if base_process.poll() is not None: break
		# If the induction step has found a proof, we need to check if the base case has reached the same k.
		if induction_process.poll() is not None and base_step_k >= induction_step_k: break
		if old_base_step_k != base_step_k:
			if base_step_k > 1 and print_smt_time:
				print("Runtime SMT-solver: " + "{0:0.2f}".format(identify_smt_time(base_step_output, True)) + "s")
			print("Base step k = " + str(base_step_k))
		if old_induction_step_k != induction_step_k:
			if induction_step_k > 1 and print_smt_time:
				print("Runtime SMT-solver: " + "{0:0.2f}".format(identify_smt_time(induction_step_output, True)) + "s")
			print("Induction step k = " + str(induction_step_k))
		# Don't overload the CPU with busy waiting.
		time.sleep(POLL_INTERVAL)
	# Killing the remaining process, if necessary.
	if base_process.poll()      is None: base_process.kill()
	if induction_process.poll() is None: induction_process.kill()
	# Fetches the outputs of the processes and passes it on to the output interpreter.
	base_step_result      = interprete_base_step_output(base_step_output)
	induction_step_result = interprete_induction_step_output(induction_step_output)
	if base_step_result == False and induction_step_result == None:
		return False
	elif induction_step_result == True and base_step_result == None:
		return True
	else:
		return None

def insert_k_into_callstring(callstring: list, k: int):
	"""
	Replaces the position of the unwind number with the given k in the callstring.
	:param callstring: The callstring in which to replace the generic k.
	:param k: The specific k to write.
	:return: The new callstring.
	:rtype: list of str
	"""
	return [c.replace(VERIFIER_KINCREMENT_STRING, str(k)) for c in callstring]

def insert_k_into_induction_file(file_induction: str, k: int):
	"""
	Inserts the current unwind number into the file such that the property is only checked once the iteration number
	has reached k.
	:param file_induction: The file the verifier run on in the previous iteration.
	:param k: The new k.
	:return: A filename whose content is the C-code for the updated iteration number k.
	"""
	if k > 1:
		# Parses the file (again) and identifies the main function (again). Not super elegant, could surely be improved.
		with open(file_induction) as file:
			ast = GnuCParser().parse(file.read())
		main_function = CAnalyzer(ast).identify_function(MAIN_FUNCTION_NAME)
		CTransformer(ast).replace_initial_value("const unsigned int k = " + str(k - 1) + ";", main_function, k)
		output_file = tempfile.NamedTemporaryFile(delete=False, suffix=".c")
		output_file.write(bytes(GnuCGenerator().visit(ast), "utf-8"))
		return output_file.name
	else:
		return file_induction

def run_kinduction_bmc(file_base: str, file_induction: str, timelimit: int=None, print_smt_time:bool=False):
	"""
	Runs the k-induction algorithm on the given files. Starts two processes, one for the base step, one for the
	induction step. Returns the answer to the verification task as soon as one of the processes stop. Note that this
	function call may run indefinitely. This function shall be used if there is no incremental BMC option for the
	underlying verifier, hence incremental BMC will be simulated.
	:param file_base: The location of the file to run the base step on.
	:param file_induction: The location of the file to run the inductive step on.
	:param timelimit: An optional limit on the CPU-time, in seconds.
	:param print_smt_time: Whether to print out the time that was spent on SMT-solving.
	:return: Either True, False or None (in case no definite answer could be given).
	"""
	base_step_k        = 0
	induction_step_k   = 0
	base_process       = None
	base_out_file      = None
	induction_process  = None
	induction_out_file = None
	while True:
		# Fetches output from the base process and checks if a counterexample could be given. If not, increases k.
		if base_process is None or base_process.poll() is not None:
				if base_out_file:
					base_out_file.seek(0)
					base_out = base_out_file.read().decode("utf-8")
				else:
					base_out = ""
				if base_out_file and interprete_base_step_output(base_out) == False:
					return False
				else:
					base_step_k += 1
					if base_step_k > 1 and print_smt_time:
						print("Runtime SMT-solver: " + "{0:0.2f}".format(identify_smt_time(base_out)) + "s")
					print("Base step k = " + str(base_step_k))
					base_call     = insert_k_into_callstring(VERIFIER_BASE_CALL, base_step_k)
					base_out_file = tempfile.TemporaryFile()
					base_process  = subprocess.Popen(base_call + [file_base], stdout=base_out_file)
		# Fetches output from the induction process and checks if a proof could be given. If not, increases k.
		if induction_process is None or induction_process.poll() is not None:
				if induction_out_file:
					induction_out_file.seek(0)
					induction_out = induction_out_file.read().decode("utf-8")
				else:
					induction_out = ""
				if base_step_k >= induction_step_k and induction_out_file and \
							interprete_induction_step_output(induction_out) is True:
					return True
				else:
					induction_step_k += 1
					if induction_step_k > 1 and print_smt_time:
						print("Runtime SMT-solver: " + "{0:0.2f}".format(identify_smt_time(induction_out)) + "s")
					print("Induction step k = " + str(induction_step_k))
					file_induction     = insert_k_into_induction_file(file_induction, induction_step_k)
					induction_call     = insert_k_into_callstring(VERIFIER_BASE_CALL, induction_step_k)
					induction_out_file = tempfile.TemporaryFile()
					induction_process  = subprocess.Popen(induction_call + [file_induction], stdout=induction_out_file)
		# Checks for a possible timeout.
		if is_timeout(timelimit): return None
		time.sleep(POLL_INTERVAL)

def add_witness_generation(input_file: str, witness_location: str):
	"""
	TODO
	:param input_file:
	:param witness_location:
	"""
	global VERIFIER_BASE_CALL, VERIFIER_INDUCTION_CALL
	witness_base_arg = f'{VERIFIER_WITNESS_GEN_ARGUMENT.replace(VERIFIER_WITNESS_FILENAME_STRING, witness_location)}'
	witness_ind_arg = f'{VERIFIER_WITNESS_GEN_ARGUMENT.replace(VERIFIER_WITNESS_FILENAME_STRING, witness_location)}'
	VERIFIER_BASE_CALL.append(witness_base_arg)
	VERIFIER_INDUCTION_CALL.append(witness_ind_arg)

def verify(input_file: str,
		   timelimit: int=None,
		   variable_moving:bool=False,
		   ignore_functions_for_variable_moving=None,
		   slicing:bool=False,
		   print_smt_time:bool=False,
		   witness_location: str=None):
	"""
	The main entry point for the k-induction algorithm. Handles everything that concerns the k-induction approach, from
	parsing, code transformation up to verifier execution and output.
	:param input_file: A filename whose file contains the C code to run k-induction on.
	:param timelimit: An optional limit on the CPU-time, in seconds.
	:param variable_moving: Whether the variable moving analysis should be applied on the input.
	:param ignore_functions_for_variable_moving: A set of functions whose contents are ignored when determining the
		usage of variables. If None, all functions are taken into account.
	:param slicing: Whether static slicing should be applied on the input.
	:param print_smt_time: Whether to print out the time that was spent on SMT-solving.
	:param witness_location: The location of an optional witness file. If None, no witness is written.
	:return: Either True, False or None (in case no definite answer could be given).
	:rtype: False, True or None
	"""
	if slicing:
		original_input_file = input_file
	else:
		original_input_file = None
	if variable_moving:
		print("Applying variable moving analysis...")
		input_file = variable_analysis_from_file(input_file, ignore_functions=ignore_functions_for_variable_moving)
	if slicing:
		print("Applying static slicing...")
		input_file = static_slicing_from_file(input_file)
	print("Preparing input files for k-Induction...")
	file_base_step      = prepare_base_step(input_file)
	file_induction_step = prepare_induction_step(input_file, original_input_file)
	if witness_location:
		print("Setting up configuration for generating witnesses...")
		add_witness_generation(input_file, witness_location)
	print("Starting k-Induction processes...")
	if VERIFIER_IS_INCREMENTAL:
		result = run_kinduction_incremental_bmc(file_base_step, file_induction_step, timelimit, print_smt_time)
	else:
		result = run_kinduction_bmc(file_base_step, file_induction_step, timelimit, print_smt_time)

	if witness_location and result is not None:
		# Takes care of incompatible GraphML representations of CBMC and CPAChecker.
		extend_from_cbmc_to_cpa_format(witness_location, input_file, result)

	if result == True:
		print("VERIFICATION SUCCESSFUL")
	elif result == False:
		print("VERIFICATION FAILED")
	else:
		print("VERIFICATION INCONCLUSIVE")
	return result

def read_config(config_file_name: str):
	"""
	Reads the YAML config from the given configuration file name into the global variables.
	:param config_file_name: The file name of the YAML configuration.
	"""
	global VERIFIER_IS_INCREMENTAL, VERIFIER_BASE_CALL, VERIFIER_INDUCTION_CALL, VERIFIER_KINCREMENT_STRING, \
		VERIFIER_FALSE_REGEX, VERIFIER_TRUE_REGEX, VERIFIER_K_REGEX, VERIFIER_SMT_TIME_REGEX_START, \
		VERIFIER_SMT_TIME_REGEX_END, POLL_INTERVAL, VERIFIER_ASSUME_FUNCTION_NAME, VERIFIER_ERROR_FUNCTION_NAME, \
		MAIN_FUNCTION_NAME, ASSERT_FUNCTION_NAME, VERIFIER_WITNESS_FILENAME_STRING, VERIFIER_WITNESS_GEN_ARGUMENT
	with open(config_file_name) as config_file:
		config = yaml.load(config_file)
		VERIFIER_IS_INCREMENTAL          = config["verifier"].get("incremental", VERIFIER_IS_INCREMENTAL)
		VERIFIER_BASE_CALL               = config["verifier"].get("base_call", VERIFIER_BASE_CALL).split()
		VERIFIER_INDUCTION_CALL          = config["verifier"].get("induction_call", VERIFIER_INDUCTION_CALL).split()
		VERIFIER_WITNESS_GEN_ARGUMENT    = config["verifier"].get("witness_gen_argument", VERIFIER_WITNESS_GEN_ARGUMENT)
		VERIFIER_WITNESS_FILENAME_STRING = config["verifier"].get("witness_filename_string",
																  VERIFIER_WITNESS_FILENAME_STRING)
		VERIFIER_KINCREMENT_STRING       = config["verifier"].get("k_increment_string", VERIFIER_KINCREMENT_STRING)
		VERIFIER_FALSE_REGEX             = config["verifier"]["output"].get("false_regex", VERIFIER_FALSE_REGEX)
		VERIFIER_TRUE_REGEX              = config["verifier"]["output"].get("true_regex", VERIFIER_TRUE_REGEX)
		VERIFIER_K_REGEX                 = config["verifier"]["output"].get("k_regex", VERIFIER_K_REGEX)
		VERIFIER_SMT_TIME_REGEX_START    = config["verifier"]["output"].get("smt_time_start_regex",
																		 VERIFIER_SMT_TIME_REGEX_START)
		VERIFIER_SMT_TIME_REGEX_END      = config["verifier"]["output"].get("smt_time_end_regex",
																		 VERIFIER_SMT_TIME_REGEX_END)
		POLL_INTERVAL                    = config["verifier"]["output"].get("poll_interval", POLL_INTERVAL)
		VERIFIER_ASSUME_FUNCTION_NAME    = config["input"].get("assume_function", VERIFIER_ASSUME_FUNCTION_NAME)
		VERIFIER_ERROR_FUNCTION_NAME     = config["input"].get("error_function", VERIFIER_ERROR_FUNCTION_NAME)
		MAIN_FUNCTION_NAME               = config["input"].get("main_function", MAIN_FUNCTION_NAME)
		ASSERT_FUNCTION_NAME             = config["input"].get("assert_function", ASSERT_FUNCTION_NAME)

def check_validity(args: Namespace):
	"""
	Checks the validity of the given argument configuration.
	:param args: The arguments that the user passed to the program.
	:return: True iff the arguments are valid, i.e. are not inconsistent and are applicable.
	:rtype: Boolean
	"""
	# TODO
	return args

def __main__():
	DESCRIPTION = "Runs k-induction on a given C-file by utilizing an (incremental) bounded model checker. " \
				  "Currently, some constraints are imposed on the C code: It has to contain the entry function named " \
				  "\"main\", inside which the loop over whom the k-Induction shall be run is located. The " \
				  "verification task shall be given by a __VERIFIER_error() call and has to be guarded by one if " \
				  "statement that contains the verification condition. The external verifier can be configured via " \
				  "the config file verifier.config.\n" \
				  "Two static optimization approaches can be optionally enabled. For variable moving, ctags is " \
				  "required to be present in the PATH. For slicing, framac is required to be present in the PATH."
	parser = argparse.ArgumentParser(description=DESCRIPTION)
	parser.add_argument("input", type=str, help="The C input file to verify.")
	parser.add_argument("-c", "--config", required=True, type=str, help="The verifier configuration file.")
	parser.add_argument("-t", "--timelimit", type=int, help="The maximum CPU-time [s] for the verification.")
	parser.add_argument("--variable-moving", action="store_true", help="Moves variables to the most local scope prior "
																	   "to verification.")
	parser.add_argument("--ignore-functions-for-variable-moving", type=str, help="An optional list of function names "
																				 "that are ignored when determining "
																				 "whether a variable can be moved or "
																				 "not. Delimited by \',\'.")
	parser.add_argument("--slicing", action="store_true", help="Applies static slicing for the reachability of the "
															   "error location prior to verification.")
	parser.add_argument("--smt-time", action="store_true", help="Prints out the time that was spent on SMT-solving.")
	parser.add_argument("-w", "--witness", type=str, default=None, help="Generates a witness for the verification at "
																		"the specified file location. Currently tested "
																		"only for CBMC.")

	args = parser.parse_args()

	# Registers signal handler so we can kill all of our child processes.
	signal.signal(signal.SIGINT, signal_handler)
	signal.signal(signal.SIGQUIT, signal_handler)
	signal.signal(signal.SIGABRT, signal_handler)
	signal.signal(signal.SIGTERM, signal_handler)

	# Reads config into global variables.
	read_config(args.config)

	# Checks the given parameters for validity.
	if not check_validity(args):
		exit(1)

	if args.ignore_functions_for_variable_moving:
		ignore_functions = set([str(item) for item in args.ignore_functions_for_variable_moving.split(',')])
	else:
		ignore_functions = set()
	# Runs the verification task.
	verify(args.input,
		   args.timelimit,
		   args.variable_moving,
		   ignore_functions,
		   args.slicing,
		   args.smt_time,
		   args.witness)

	exit(0)

__main__()