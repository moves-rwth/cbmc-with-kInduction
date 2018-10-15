import fcntl
import os
import subprocess
import argparse
import shutil
import tempfile
import re
import time

from pycparserext.ext_c_parser import GnuCParser
from pycparserext.ext_c_generator import GnuCGenerator
from canalyzer import *
from ctransformer import *

# TODO move to config
VERIFIER_IS_INCREMENTAL       = False
#VERIFIER_BASE_CALL            = ["cbmc-ps.sh", "--incremental-check main.X", "--no-unwinding-assertions"]
#VERIFIER_INDUCTION_CALL       = ["cbmc-ps.sh", "--incremental-check main.X", "--stop-when-unsat", "--no-unwinding-assertions"]
VERIFIER_BASE_CALL            = ["cbmc.sh", "--unwindset main.X:KINCREMENT", "--no-unwinding-assertions"]
VERIFIER_INDUCTION_CALL       = ["cbmc.sh", "--unwindset main.X:KINCREMENT", "--no-unwinding-assertions"]
VERIFIER_KINCREMENT_STRING    = "KINCREMENT"
VERIFIER_FALSE_REGEX          = "VERIFICATION FAILED"
VERIFIER_TRUE_REGEX           = "VERIFICATION SUCCESSFUL"
VERIFIER_K_REGEX              = "VERIFICATION FAILED|VERIFICATION SUCCESSFUL"
VERIFIER_ASSUME_FUNCTION_NAME = "__VERIFIER_assume"
MAIN_FUNCTION_NAME            = "main"

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

def prepare_induction_step(input_file: str):
	"""
	Prepares the input C file for the execution of the induction step. It parses the code, havocs the main loop
	variables and adds the property assumption to the beginning of the loop body. When finished, the C code is written
	to a temporary working file which is then returned.
	:param input_file: The input C file to prepare.
	:return: The location of the prepared C file for the induction step.
	:rtype: str
	"""
	with open(input_file) as file:
		ast = GnuCParser().parse(file.read())
	analyzer    = CAnalyzer(ast)
	transformer = CTransformer(ast)
	# Transforms code to be a little bit more uniform and easier to work with.
	# transformer.deanonymize_aggregates()
	# Identifies main components of the code.
	try:
		main_function = analyzer.identify_function(MAIN_FUNCTION_NAME)
		main_loop     = analyzer.identify_main_loop()
		declarations  = analyzer.identify_declarations_of_modified_variables(main_loop.stmt, main_function)
		property      = analyzer.identify_property()
	except (NoMainFunctionException,
			NoMainLoopException,
			MultipleMainLoopsException,
			UnidentifiableVariableTypeException) as err:
		print(err)
		sys.exit(1)
	# Creates new code and functions to be added.
	try:
		havoc_block = transformer.create_havoc_block(declarations)
	except NonSvCompTypeException as err:
		print(err)
		sys.exit(1)
	negated_property = transformer.add_to_expression(property, "!")
	assume_property  = transformer.create_function_call(VERIFIER_ASSUME_FUNCTION_NAME, negated_property)
	# Adds some code to emulate incremental BMC: Only check property if we are in the k-th main loop iteration.
	if not VERIFIER_IS_INCREMENTAL:
		k_initialization = transformer.from_code("const unsigned int k = 1;").block_items[0]
		i_initialization = transformer.from_code("unsigned int i = 0;").block_items[0]
		i_increment      = transformer.from_code("i++;").block_items[0]
		k_property       = transformer.add_to_expression(property,
														 "&&",
														 c_ast.ExprList(transformer.from_code("(i == k)").block_items))
		transformer.replace_property(k_property)
		transformer.insert(k_initialization, before=main_loop)
		transformer.insert(i_initialization, before=main_loop)
		transformer.insert(i_increment, before=main_loop.stmt.block_items[0])
	# Inserts new code pieces.
	transformer.insert(havoc_block, before=main_loop)#.stmt.block_items[0]) # TODO is this correct?
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

def identify_k(output: str):
	"""
	Identifies the current iteration number given the verifier output.
	:param output: A (partial) output of a verifier.
	:return: The iteration number.
	:rtype: int
	"""
	# Returns the number of occurences of the verifier "k" regex plus one (as we start with k=1).
	return len(re.compile(VERIFIER_K_REGEX).findall(output)) + 1

def run_kinduction_incremental_bmc(file_base: str, file_induction: str):
	"""
	Runs the k-Induction algorithm on the given files. Starts two processes, one for the base step, one for the
	induction step. Returns the answer to the verification task as soon as one of the processes stop. Note that this
	function call may run indefinitely. This function shall only be used if the underlying verifier supports an
	incremental BMC setting.
	:param file_base: The location of the file to run the base step on.
	:param file_induction: The location of the file to run the inductive step on.
	:return: Either True, False or None (in case no definite answer could be given).
	"""
	# Starts both processes.
	base_process      = subprocess.Popen(VERIFIER_BASE_CALL + [file_base], stdout=subprocess.PIPE)
	induction_process = subprocess.Popen(VERIFIER_INDUCTION_CALL + [file_induction], stdout=subprocess.PIPE)
	# Sets stdout to non-blocking IO, so we are able to fetch intermediate data without blocking if no data was read.
	fl = fcntl.fcntl(base_process.stdout, fcntl.F_GETFL)
	fcntl.fcntl(base_process.stdout, fcntl.F_SETFL, fl | os.O_NONBLOCK)
	fl = fcntl.fcntl(induction_process.stdout, fcntl.F_GETFL)
	fcntl.fcntl(induction_process.stdout, fcntl.F_SETFL, fl | os.O_NONBLOCK)
	base_step_output      = bytes()
	induction_step_output = bytes()
	base_step_k           = 0
	induction_step_k      = 0
	# Busy waiting until one of the processes finishes. Already fetching intermediate output here for some processing.
	while True:
		# Identifies new output, if any.
		base_step_output      += base_process.stdout.readline()
		induction_step_output += induction_process.stdout.readline()
		# Checks if one of the processes has reached a new k, and prints it.
		old_base_step_k      = base_step_k
		old_induction_step_k = induction_step_k
		base_step_k          = identify_k(base_step_output.decode("utf-8"))
		induction_step_k     = identify_k(induction_step_output.decode("utf-8"))
		if old_base_step_k != base_step_k:
			print("Base step k = " + str(base_step_k))
		if old_induction_step_k != induction_step_k:
			print("Induction step k = " + str(induction_step_k))
		# Quits if the base process has found a counterexample.
		if base_process.poll() is not None: break
		# If the induction step has found a proof, we need to check if the base case has reached the same k.
		if induction_process.poll() is not None and base_step_k >= induction_step_k: break
	# Killing the remaining process, if necessary.
	if base_process.poll()      is None: base_process.kill()
	if induction_process.poll() is None: induction_process.kill()
	# Reads the rest of the process output that may be accumulated after the last readline() call.
	for line in base_process.stdout: base_step_output += line
	for line in induction_process.stdout: induction_step_output += line
	# Fetches the outputs of the processes and passes it on to the output interpreter.
	base_step_result      = interprete_base_step_output(base_step_output.decode("utf-8"))
	induction_step_result = interprete_induction_step_output(induction_step_output.decode("utf-8"))
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
		with open(file_induction) as file:
			ast = GnuCParser().parse(file.read())
		main_function = CAnalyzer(ast).identify_function(MAIN_FUNCTION_NAME)
		CTransformer(ast).replace_initial_value("const unsigned int k = " + str(k - 1) + ";", main_function, k)
		output_file = tempfile.NamedTemporaryFile(delete=False, suffix=".c")
		output_file.write(bytes(GnuCGenerator().visit(ast), "utf-8"))
		return output_file.name
	else:
		return file_induction

def run_kinduction_bmc(file_base: str, file_induction: str):
	"""
	Runs the k-Induction algorithm on the given files. Starts two processes, one for the base step, one for the
	induction step. Returns the answer to the verification task as soon as one of the processes stop. Note that this
	function call may run indefinitely. This function shall be used if there is no incremental BMC option for the
	underlying verifier, hence incremental BMC will be simulated.
	:param file_base: The location of the file to run the base step on.
	:param file_induction: The location of the file to run the inductive step on.
	:return: Either True, False or None (in case no definite answer could be given).
	"""
	base_step_k       = 0
	induction_step_k  = 0
	base_process      = None
	induction_process = None
	while True:
		if base_process is None or base_process.poll() is not None:
			base_step_output = bytes()
			if base_process is not None:
				for line in base_process.stdout: base_step_output += line
			if interprete_base_step_output(base_step_output.decode("utf-8")) == False:
				return False
			else:
				base_step_k += 1
				print("Base step k = " + str(base_step_k))
				base_call    = insert_k_into_callstring(VERIFIER_BASE_CALL, base_step_k)
				base_process = subprocess.Popen(base_call + [file_base], stdout=subprocess.PIPE)
		if induction_process is None or induction_process.poll() is not None:
			induction_step_output = bytes()
			if induction_process is not None:
				for line in induction_process.stdout: induction_step_output += line
			if interprete_induction_step_output(induction_step_output.decode("utf-8")) == True:
				return True
			else:
				induction_step_k += 1
				print("Induction step k = " + str(induction_step_k))
				file_induction    = insert_k_into_induction_file(file_induction, induction_step_k)
				induction_call    = insert_k_into_callstring(VERIFIER_BASE_CALL, induction_step_k)
				induction_process = subprocess.Popen(induction_call + [file_induction], stdout=subprocess.PIPE)
		time.sleep(1)

def verify(input_file):
	"""
	The main entry point for the k-Induction algorithm. Handles everything that concerns the k-Induction approach, from
	parsing, code transformation up to verifier execution and output.
	input_file: A file containing the C-code to run k-Induction on.
	:return: Either True, False or None (in case no definite answer could be given).
	"""
	print("Preparing input files for k-Induction...")
	file_base_step      = prepare_base_step(input_file)
	file_induction_step = prepare_induction_step(input_file)
	print("Starting k-Induction processes...")
	if VERIFIER_IS_INCREMENTAL:
		result = run_kinduction_incremental_bmc(file_base_step, file_induction_step)
	else:
		result = run_kinduction_bmc(file_base_step, file_induction_step)
	if result == True:
		print("VERIFICATION SUCCESSFUL")
	elif result == False:
		print("VERIFICATION FAILED")
	else:
		print("VERIFICATION INCOMPLETE")
	return result

def __main__():
	DESCRIPTION = "Runs k-Induction on a given C-file by utilizing an incremental bounded model checker. " \
				  "Currently, some constraints are imposed on the C code: It has to contain the entry function named " \
				  "\"main\", inside which the loop over whom the k-Induction shall be run is located. The " \
				  "verification task shall be given by a __VERIFIER_error() call and has to be guarded by one or " \
				  "more if statements that contain the verification condition. " \
				  "The external verifier can be configured via the config file verifier.config."
	parser = argparse.ArgumentParser(description=DESCRIPTION)
	parser.add_argument("input", type=str)

	args = parser.parse_args()
	input_file = args.input

	verify(input_file)

	exit(0)

__main__()