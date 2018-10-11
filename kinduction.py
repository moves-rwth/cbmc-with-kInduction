import subprocess
import argparse
import shutil
import tempfile
import re
from pycparser import parse_file
from canalyzer import *
from ctransformer import *

# TODO move to config
VERIFIER_BASE_CALL            = "cbmc-ps --incremental --no-unwinding-assertions"
VERIFIER_INDUCTION_CALL       = "cbmc-ps --incremental --stop-when-unsat --no-unwinding-assertions"
VERIFIER_FALSE_REGEX          = "VERIFICATION FAILED"
VERIFIER_TRUE_REGEX           = "VERIFICATION SUCCESSFUL"
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
	ast         = parse_file(input_file)
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
		havoc_block      = transformer.create_havoc_block(declarations)
		negated_property = transformer.create_negated_expression(property)
		assume_property  = transformer.create_function_call(VERIFIER_ASSUME_FUNCTION_NAME, negated_property)
	except NonSvCompTypeException as err:
		print(err)
		sys.exit(1)
	# Inserts new code pieces.
	transformer.insert(assume_property, before=main_loop.stmt.block_items[0])
	transformer.insert(havoc_block, before=main_loop)
	# Writes the transformed code to a temporary file.
	output_file = tempfile.NamedTemporaryFile(delete=False, suffix=".c")
	print("k-Induction main function after transformation:")
	print(c_generator.CGenerator().visit(main_function))
	output_file.write(bytes(c_generator.CGenerator().visit(ast), "utf-8"))
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

def run_kinduction(file_base: str, file_induction: str):
	"""
	Runs the k-Induction algorithm on the given files. Starts two processes, one for the base step, one for the
	induction step. Returns the answer to the verification task as soon as one of the processes stop. Note that this
	function call may run indefinitely.
	:param file_base: The location of the file to run the base step on.
	:param file_induction: The location of the file to run the inductive step on.
	:return: Either True, False or None (in case no definite answer could be given).
	"""
	# Starts both processes.
	base_process      = subprocess.Popen([VERIFIER_BASE_CALL + " " + file_base], shell=True, stdout=subprocess.PIPE)
	induction_process = subprocess.Popen([VERIFIER_INDUCTION_CALL + " " + file_induction], shell=True, stdout=subprocess.PIPE)
	# Busy waiting until one of the processes finishes.
	while base_process.poll() == None and induction_process.poll() == None: pass
	# Killing the remaining process, if necessary.
	if base_process.poll()      == None: base_process.kill()
	if induction_process.poll() == None: induction_process.kill()
	# Fetches the outputs of the processes and passes it on to the output interpreter.
	base_step_output      = base_process.communicate()[0]
	induction_step_output = induction_process.communicate()[0]
	base_step_result      = interprete_base_step_output(base_step_output.decode("utf-8"))
	induction_step_result = interprete_induction_step_output(induction_step_output.decode("utf-8"))
	if base_step_result == False:
		return False
	elif induction_step_result == True:
		return True
	else:
		return None

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
	print("Running k-Induction processes...")
	result = run_kinduction(file_base_step, file_induction_step)
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