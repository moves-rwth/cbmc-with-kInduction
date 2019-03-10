"""
This module uses Frama-C to apply static slicing on the given input. In its default setting, it slices on the
reachability of __VERIFIER_error and assert calls.
"""

import os
import re
import subprocess
import sys
import tempfile

from pycparser import c_ast
from pycparserext.ext_c_generator import GnuCGenerator
from pycparserext.ext_c_parser import GnuCParser, FuncDeclExt, TypeDeclExt


class SwitchDeclarationFinderAndMover(c_ast.NodeVisitor):
	"""
	Identifies declarations inside switch-blocks (but not inside their cases). Moves them from the switch block to a new
	block just outside of the switch statement.
	"""
	def __init__(self):
		self.parents = []

	def generic_visit(self, node):
		if type(node) != FuncDeclExt and type(node) != TypeDeclExt:
			self.parents.append(node)
			for c_name, c in node.children():
				self.visit(c)
			self.parents = self.parents[:-1]

	def visit_Switch(self, node):
		if type(node.stmt) is c_ast.Compound:
			declarations = []
			new_block_items = []
			for item in node.stmt.block_items:
				if type(item) is c_ast.Decl:
					declarations.append(item)
				else:
					new_block_items.append(item)
			node.stmt.block_items = new_block_items
			if len(self.parents) > 0:
				new_switch_block = c_ast.Compound(declarations + [node])
				if hasattr(self.parents[-1], "block_items"):
					index = self.parents[-1].block_items.index(node)
					self.parents[-1].block_items.remove(node)
					self.parents[-1].block_items.insert(index, new_switch_block)
				elif hasattr(self.parents[-1], "iftrue") and self.parents[-1].iftrue == node:
					self.parents[-1].iftrue = new_switch_block
				elif hasattr(self.parents[-1], "iffalse") and self.parents[-1].iffalse == node:
					self.parents[-1].iftrue = new_switch_block
				else:
					# TODO: The three cases above most likely don't cover all possible contexts in which a switch can
					# occur. This works on both case studies, so sufficient as of now. But for future use, one might
					# want to evaluate from the C standard which contexts are generaly possible!
					print("Can not identify the context of a switch statement that is contained in " +
						  str(type(self.parents[-1])), file=sys.stderr)
					exit(1)

def move_switch_local_variables(ast: c_ast):
	"""
	Moves declaration of variables that are local two switch blocks (e.g. y in switch (0) { int y; case 0: break; }) out
	of the switch block. Frama-C produces such declarations which may result in bugs for some verifiers. Works in-place.
	:param ast: The AST to work on.
	"""
	SwitchDeclarationFinderAndMover().visit(ast)

def static_slicing_from_ast(ast: c_ast):
	"""
	Executes static slicing on the given AST.
	:param ast: A PyCParser AST.
	:return: A PyCParser AST on which static slicing has been applied.
	:rtype: c_ast
	"""
	ast_file = tempfile.NamedTemporaryFile(delete=False, suffix=".c")
	ast_file.write(bytes(GnuCGenerator().visit(ast), "utf-8"))
	sliced_file = static_slicing_from_file(ast_file.name)
	with open(sliced_file, "r") as slice:
		return GnuCParser().parse(slice.read())

def static_slicing_from_file(input_file: str, output: str=None, slice_funcs: list=None):
	"""
	Executes static slicing from a given file name. Writes to the output file if given, otherwise creates a	temporary
	file and writes the results to it.
	:param input_file: The location of the input file.
	:param output: An optional file location.
	:param slice_funcs: The functions to slice for their reachability. If None, the default is __VERIFIER_error.
	:return: The file name of the output file.
	:rtype: str
	"""
	# Need a temporary file because Frama-C does not allow to only print out the C code.
	slice_file = tempfile.NamedTemporaryFile(delete=False, suffix=".c")
	if not slice_funcs or len(slice_funcs) == 0:
		slice_funcs = ["__VERIFIER_error"]
	with open(input_file, "r") as input:
		# Only keep assume statements if there are any, otherwise Frama-C complains.
		if "__VERIFIER_assume" in input.read().replace("extern int __VERIFIER_assume(int);", ""):
			slice_funcs.append("__VERIFIER_assume")
	# /dev/null supresses output of Frama-C.
	with open(os.devnull, "w") as devnull:
		subprocess.call(["frama-c", "-slice-calls", ",".join(slice_funcs), input_file, "-then-on", "Slicing export",
						 "-print", "-ocode", slice_file.name], stdout=devnull)
	slice = slice_file.read().decode("utf-8")
	# Removes comments of the form /**/ as GnuCParser does not seem to like them.
	slice = re.sub("/\*.*?\*/", "", slice)
	# Parses the slice and original to allow for the process of moving switch-local variables out of switch blocks.
	slice_ast = GnuCParser().parse(slice)
	# Re-adds possibly lost __VERIFIER_assume statements.
	move_switch_local_variables(slice_ast)
	# Output printing.
	if not output:
		output_file = tempfile.NamedTemporaryFile(delete=False, suffix=".c")
		output = open(output_file.name, "w")
	output.write(GnuCGenerator().visit(slice_ast))
	return output.name

def main():
	if len(sys.argv) > 2:
		output = open(sys.argv[2], "w")
	else:
		output = sys.stdout
	static_slicing_from_file(sys.argv[1], output)

if __name__ == '__main__':
	main()