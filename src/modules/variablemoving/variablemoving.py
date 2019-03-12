"""
This module performs a variable scope analysis. It moves global variables to their most local scope, and removes unused
global variables. This can be useful for k-induction due to the reduced global state space after running the analysis.
Note that this script was specifically developed for the two ECC and DSR Ford case studies and likely won't work on
other input code.
"""

import os
import sys
import io
import tempfile

from pycparser import c_ast
from pycparserext.ext_c_parser import GnuCParser, FuncDeclExt, TypeDeclExt
from pycparserext.ext_c_generator import GnuCGenerator

class StructNamesCollector(c_ast.NodeVisitor):
	def __init__(self):
		self.structs = set()

	def generic_visit(self, node):
		if type(node) != FuncDeclExt and type(node) != TypeDeclExt:
			for c in node:
				self.visit(c)

	def visit_Struct(self, node):
		self.structs.add(node.name)

	def visit_TypeDecl(self, node):
		if type(node.type) is c_ast.Struct:
			self.structs.add(node.declname)

class FuncDefVisitor(c_ast.NodeVisitor):
	def __init__(self, funcs, variable):
		self.funcs = funcs
		self.variable = variable

	def generic_visit(self, node):
		if type(node) != FuncDeclExt and type(node) != TypeDeclExt:
			for c in node:
				self.visit(c)

	def visit_FuncDef(self, node):
		if contains_variable(node, self.variable):
			self.funcs.append(node.decl.name)

class FuncDefAddVarDeclVisitor(c_ast.NodeVisitor):
	def __init__(self, func, variable_declaration, structs):
		self.func = func
		self.variable_declaration = variable_declaration
		if variable_declaration.storage is not None:
			variable_declaration.storage.append("static")
		else:
			variable_declaration.storage = ["static"]
		if self.variable_declaration.init is None:
			if "extern" in variable_declaration.storage:
				variable_declaration.storage.remove("extern")
			if type(self.variable_declaration.type) is c_ast.ArrayDecl or \
				any(x in structs for x in self.variable_declaration.type.type.names):
				self.variable_declaration.init = c_ast.InitList(exprs=[c_ast.Constant(type="int", value="0")])
			else:
				self.variable_declaration.init = c_ast.Constant(type="int", value="0")

	def generic_visit(self, node):
		if type(node) != FuncDeclExt and type(node) != TypeDeclExt:
			for c in node:
				self.visit(c)

	def visit_FuncDef(self, node):
		if node.decl.name == self.func:
			node.body.block_items.insert(0, self.variable_declaration)

class IDVisitor(c_ast.NodeVisitor):
	def __init__(self, variable):
		self.variable = variable
		self.contains = False

	def generic_visit(self, node):
		if type(node) != FuncDeclExt and type(node) != TypeDeclExt:
			for c in node:
				self.visit(c)

	def visit_ID(self, node):
		if self.variable == node.name:
			self.contains = True

class DeclRHSVisitor(c_ast.NodeVisitor):
	def __init__(self, variable):
		self.variable = variable
		self.contains = False

	def generic_visit(self, node):
		if type(node) != FuncDeclExt and type(node) != TypeDeclExt:
			for c in node:
				self.visit(c)

	def visit_Decl(self, node):
		for i, child in enumerate(node.children(), []):
			if i != "name" and IDVisitor(self.variable).visit(child):
				self.contains = True


class DeclRemovalVisitor(c_ast.NodeVisitor):
	def __init__(self, variable):
		self.variable = variable
		self.current_parent = None
		self.variable_declaration = None

	def generic_visit(self, node):
		if type(node) != FuncDeclExt and type(node) != TypeDeclExt:
			oldparent = self.current_parent
			self.current_parent = node
			for c_name, c in node.children():
				self.visit(c)
			self.current_parent = oldparent

	def visit_Decl(self, node):
		if node.name == self.variable:
			for i, child in enumerate(self.current_parent.ext or []):
				if child == node:
					self.current_parent.ext.remove(child)
					self.variable_declaration = child

class StatementRemovalVisitor(c_ast.NodeVisitor):
	def __init__(self, variable):
		self.variable = variable
		self.current_parent = None

	def generic_visit(self, node):
		if type(node) != FuncDeclExt and type(node) != TypeDeclExt:
			for c in node:
				self.visit(c)

	def visit_Compound(self, node):
		for i, child in enumerate(node.block_items or []):
			child_io = io.StringIO()
			child.show(child_io)
			child_string = child_io.getvalue()
			if(("ID: " + self.variable) in child_string):
				node.block_items.remove(child)

class FuncCallRemovalVisitor(c_ast.NodeVisitor):
	def __init__(self, variable):
		self.variable = variable
		self.current_parent = None

	def generic_visit(self, node):
		if type(node) != FuncDeclExt and type(node) != TypeDeclExt:
			oldparent = self.current_parent
			self.current_parent = node
			for c_name, c in node.children():
				self.visit(c)
			self.current_parent = oldparent

	def visit_FuncCall(self, node):
		if(node.args is not None):
			for i, child in enumerate(node.args.exprs or []):
				child_io = io.StringIO()
				child.show(child_io)
				child_string = child_io.getvalue()
				if(("ID: " + self.variable) in child_string):
					self.current_parent.block_items.remove(node)

def contains_variable(function_body, variable):
	v = IDVisitor(variable)
	v.visit(function_body)
	return v.contains

def contains_variable_as_rhs(decl, variable):
	v = DeclRHSVisitor(variable)
	v.visit(decl)
	return v.contains

def get_usages_for_variable(ast, variable):
	funcs = []
	v = FuncDefVisitor(funcs, variable)
	v.visit(ast)
	for i, child in enumerate(ast.ext or []):
		if type(child) is not c_ast.FuncDef and contains_variable(child, variable):
			funcs.append("")
	return funcs

def determine_variables_to_be_moved(var_to_funcs, ignore_functions=None):
	change_var_to_funcs = {}
	for var in var_to_funcs.keys():
		if len(var_to_funcs[var]) == 1 and ignore_functions and var_to_funcs[var][0] not in ignore_functions:
			change_var_to_funcs[var] = var_to_funcs[var]
	return change_var_to_funcs

def determine_variables_to_be_removed(var_to_funcs, ignore_functions=None):
	vars = set()
	for var in var_to_funcs.keys():
		if len(var_to_funcs[var]) == 0 or (len(var_to_funcs[var]) == 1 and ignore_functions and
										   var_to_funcs[var][0] in ignore_functions):
			vars.add(var)
	return vars

def remove_statements_with_variables(ast, vars):
	for var in vars:
		v = StatementRemovalVisitor(var)
		v.visit(ast)
		v = FuncCallRemovalVisitor(var)
		v.visit(ast)
	return ast

def remove_variable_declarations(ast, vars):
	variable_declarations = {}
	for var in vars:
		v = DeclRemovalVisitor(var)
		v.visit(ast)
		variable_declarations[var] = v.variable_declaration
	return ast, variable_declarations

def move_variables_to_func(ast, var_to_func):
	ast, variable_declarations = remove_variable_declarations(ast, var_to_func.keys())
	structs_collector = StructNamesCollector()
	structs_collector.visit(ast)
	for var in var_to_func.keys():
		v = FuncDefAddVarDeclVisitor(var_to_func[var][0],
									 variable_declarations[var],
									 {"struct"}.union(structs_collector.structs))
		v.visit(ast)
	return ast

def remove_variables(ast, vars):
	ast, variable_declarations = remove_variable_declarations(ast, vars)
	ast = remove_statements_with_variables(ast, vars)
	return ast

def variable_analysis_from_ast(ast: c_ast, ignore_functions=None):
	"""
	Executes the variable analysis on the given AST.
	:param ast: A PyCParser AST.
	:param ignore_functions: A set of functions whose contents are ignored when determining the usage of variables. If
		None, all functions are taken into account.
	:return: A PyCParser AST on which the analysis has been applied.
	:rtype: c_ast
	"""
	# Writes AST content to file, in order to run ctags on it.
	ast_file = tempfile.NamedTemporaryFile(delete=False, suffix=".c")
	ast_file.write(bytes(GnuCGenerator().visit(ast), "utf-8"))
	# Runs ctags to extract global variables from the given AST.
	vars = os.popen("ctags -x --c-kinds=v --file-scope=no " + ast_file.name +
					" | grep -v volatile | awk '{print $1;}'").read().splitlines()

	# For each global variable, finds every function that accesses it.
	var_to_funcs = {}
	for var in vars:
		var_to_funcs[var] = get_usages_for_variable(ast, var)
	var_and_funcs_to_be_moved = determine_variables_to_be_moved(var_to_funcs, ignore_functions)
	vars_to_be_removed = determine_variables_to_be_removed(var_to_funcs, ignore_functions)

	if len(vars_to_be_removed) > 0:
		ast = remove_variables(ast, vars_to_be_removed)
		print("Removed " + str(len(vars_to_be_removed)) + " global variables (" +
			  "{:.1%}".format((len(vars_to_be_removed)/len(vars))) + ") and their retrospectively added statements.")
	if len(var_and_funcs_to_be_moved) > 0:
		ast = move_variables_to_func(ast, var_and_funcs_to_be_moved)
		print("Moved " + str(len(var_and_funcs_to_be_moved)) + " global variables ("
			  + "{:.1%}".format((len(var_and_funcs_to_be_moved)/len(vars))) + ") to their local function scope.")
	return ast

def variable_analysis_from_file(input_file: str, output: str=None, ignore_functions=None):
	"""
	Executes the variable analysis from a given file name. Writes to the output file if given, otherwise creates a
	temporary file and writes the results to it.
	:param input_file: The location of the input file.
	:param output: An optional file location.
	:param ignore_functions: A set of functions whose contents are ignored when determining the usage of variables. If
		None, all functions are taken into account.
	:return: The file name of the output file.
	:rtype: str
	"""
	with open(input_file) as input:
		ast = GnuCParser().parse(input.read())
		changed_ast = variable_analysis_from_ast(ast, ignore_functions)
		if not output:
			output_file = tempfile.NamedTemporaryFile(delete=False, suffix=".c")
			output = open(output_file.name, "w")
		output.write(GnuCGenerator().visit(changed_ast))
		return output.name

def main():
	if len(sys.argv) > 2:
		output = open(sys.argv[2], "w")
	else:
		output = sys.stdout
	variable_analysis_from_file(sys.argv[1], output)

if __name__ == '__main__':
	main()