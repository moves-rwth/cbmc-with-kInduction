"""
Performs various (static) C code analyses using pycparser.
Currently supported analyses:
	- Variable occurrence:
		Lists all variables that occur in a function.
	- Modified variable occurrence:
		Lists all modified variables that occur in a function.
	- Variable type inference
		Finds the type of a variable, resolves typedefs.
	- Variable modification check
		Checks whether a variable is modified in a function.
	- Function identification
		Searches for a function definition given a function name.
	- Main loop identification
		Searches for an unbounded main loop in the main function.
	- Property identification
		Searches for an assert or SV-comp property.
"""

import copy
from pycparser import c_ast, c_generator

VERIFIER_ERROR_FUNCTION_NAME = "__VERIFIER_error"
ASSERT_FUNCTION_NAME         = "assert"

########################################################################################################################
# Exceptions                                                                                                           #
########################################################################################################################

class MultipleMainLoopsException(Exception):
	"""
	Indicates that multiple (unbounded) main loops are present in the entry function of the given program.
	"""
	def __init__(self, loops: list, main_function_name: str="main"):
		self.loops = loops
		self.main_function_name = main_function_name

	def __str__(self):
		message = "Input program contains multiple (" + str(len(self.loops)) + ") main loops in function " \
				  + self.main_function_name + ", namely:\n"
		for i, loop in enumerate(self.loops):
			message += str(i) + ")\n" + c_generator.CGenerator().visit(loop) + "\n"
		return message

class NoMainLoopException(Exception):
	"""
	Indicates that no main loop of the identified main function name could be found.
	"""
	def __init__(self, main_function_name: str="main"):
		self.main_function_name = main_function_name

	def __str__(self):
		return "Can not find a loop within function " + self.main_function_name + "."

class NoMainFunctionException(Exception):
	"""
	Indicates that no main function of the configured main function name could be found in the AST.
	"""
	def __init__(self, main_function_name: str="main"):
		self.main_function_name = main_function_name

	def __str__(self):
		return "Can not find a function with the name " + self.main_function_name + "."

class UnidentifiableVariableTypeException(Exception):
	"""
	Indicates that a variable was encountered whose type can not be identified.
	"""
	def __init__(self, variable: str):
		self.variable = variable

	def __str__(self):
		return "Can not identify the type of variable " + self.variable + "."

class AmbiguousVariableTypeException(Exception):
	"""
	Indicates that a variable was encountered whose type can not be identified.
	"""
	def __init__(self, variable: str, types: list):
		self.variable = variable
		self.types = types

	def __str__(self):
		return "Identified multiple types for variable " +\
			   self.variable + ": " + ", ".join([str(t) for t in self.types]) + "."

########################################################################################################################
# AST Node Visitors                                                                                                    #
########################################################################################################################

class PropertyFinder(c_ast.NodeVisitor):
	"""
	Finds the verification property in the AST node and puts it into its property member.
	Searches for all compound blocks that contains a function call with the verifier error function and assert
	statements. It then assembles the property via the surrounding if statement or the arguments of the assert.
	Stores the properties as an ExprList whose elements should be understood as a conjunct.
	"""
	def __init__(self):
		self.properties = c_ast.ExprList([])
		self.parents = []

	def generic_visit(self, node):
		self.parents.append(node)
		for c_name, c in node.children():
			self.visit(c)
		self.parents = self.parents[:-1]

	def visit_FuncCall(self, node):
		# Case __VERIFIER_error was used.
		if node.name.name == VERIFIER_ERROR_FUNCTION_NAME:
			if type(self.parents[-1]) == c_ast.Compound and type(self.parents[-2]) == c_ast.If:
				if type(self.parents[-2].cond) == c_ast.ExprList:
					self.properties.exprs += self.parents[-2].cond.exprs
				else:
					self.properties.exprs.append(self.parents[-2].cond)
		# Case assert was used.
		elif node.name.name == ASSERT_FUNCTION_NAME:
			if type(node.args) == c_ast.ExprList:
				self.properties.exprs += node.args.exprs
			else:
				self.properties.exprs.append(node.args)

class TypedefCollector(c_ast.NodeVisitor):
	"""
	Collects all typedefs in a list of tuples. The first element of each tuple contains the left-hand side of the
	typedef (i.e. the overlaid types) as a list of strings, and the second element contains the new typename as a str.
	"""
	def __init__(self):
		self.typedefs = []

	def visit_Typedef(self, node):
		self.typedefs.append(node)

class DeclarationCollector(c_ast.NodeVisitor):
	"""
	Collects all declarations of variables present in the AST. Puts them into a set of tuples, first entry contains the
	declaration, the second contains the scope as a c_ast.Node.
	"""
	def __init__(self):
		self.declarations = set()
		self.parents = []

	def generic_visit(self, node):
		self.parents.append(node)
		for c_name, c in node.children():
			self.visit(c)
		self.parents = self.parents[:-1]

	def visit_Decl(self, node):
		# Searches for the scope of the variable - It is either block, function parameter, or global or an aggregate.
		scope = None
		for parent in reversed(self.parents):
			if type(parent) == c_ast.FileAST\
					or type(parent) == c_ast.Compound\
					or type(parent) == c_ast.FuncDef\
					or type(parent) == c_ast.Struct\
					or type(parent) == c_ast.Union:
				scope = parent
				break
		if scope is None:
			scope = node
		if type(node.type) != c_ast.FuncDecl:
			self.declarations.add((node, scope))
		# TODO maybe call visit again to get function params?

class LoopFinder(c_ast.NodeVisitor):
	"""
	Finds all loops in the AST node and puts them into its loops list.
	"""
	def __init__(self):
		self.loops = []

	def visit_DoWhile(self, node):
		self.loops.append(node)

	def visit_While(self, node):
		self.loops.append(node)

	def visit_For(self, node):
		self.loops.append(node)

########################################################################################################################
# Analyzer                                                                                                             #
########################################################################################################################

class CAnalyzer:
	def __init__(self, ast: c_ast.FileAST, main_function_name: str="main"):
		self.ast = ast
		self.main_function_name = main_function_name

	def identify_declarations(self, function: c_ast.FuncDef):
		"""
		Collects a list of all variables used in the function-scope - local and global. Each variable is represented by
		its declaration, in which any typedefs are already completely resolved.
		:param function: The functions to identify variables in.
		:return: A list of declarations, one for each variable.
		:rtype: list of c_ast.Decl
		"""
		declarations = []
		typedefs = self.identify_typedefs()
		collector = DeclarationCollector()
		collector.visit(self.ast)
		for declaration, scope in collector.declarations:
			self.resolve_typedefs(declaration.type, typedefs)
			if scope == function or scope == function.body or scope == self.ast:
				declarations.append(declaration)
		return declarations

	def is_variable_of_declaration_modified(self, declaration: c_ast.Decl, block: c_ast.Compound):
		"""
		Returns true iff the variable of the declaration is modified in the given compound.
		:param declaration: The declaration of the variable to check for.
		:param block: The block to which the modifications should be local.
		:return: True or false.
		"""
		if "const" in declaration.quals:
			# Constant variables can not be modified.
			return False
		else:
			# We have a non-aggregate basic C type. We can now check for variable occurrences in LHS of assignments.
			# TODO Can we actually make any assumptions about that, as pointer aliasing exists?
			# int x = 1;
			# int* y = &x;
			# *y = 2;
			# Hence, we need to check if occurring on LHS or the address is somehow taken from it. Then we return True.
			pass
		return True

	def identify_declarations_of_modified_variables(self, block: c_ast.Compound, function: c_ast.FuncDef):
		"""
		Given a PyCParser AST, returns all the variables that are modified in the compound block. Includes global
		variables that are modified through function calls.
		:parameter ast: The AST to find the variables in.
		:parameter block: An AST compound block representing the compound block to find the variables for.
		:parameter function: The function the AST compound block is contained in.
		:return: A list of declarations.
		:rtype: list of c_ast.Decl
		"""
		# First, collects a list of candidates - All global and block-local variables.
		declarations = self.identify_declarations(function)
		# Checks for each candidate whether it as actually modified, i.e. on the right-hand side of some assignment.
		# This has to respect function calls, i.e. when the variable is passed to a function, it shall be checked for
		# modification of that candidate too.
		modified_declarations = []
		for declaration in declarations:
			if self.is_variable_of_declaration_modified(declaration, block):
				modified_declarations.append(declaration)
		return modified_declarations

	def identify_function(self, function_name:str):
		"""
		Identifies the main function in the given AST.
		:param function_name: The identifier of the function to search for.
		:raise: NoMainFunctionException if no main function of the configured main function name could be identified.
		:return: The main function.
		:rtype: c_ast.FuncDef
		"""
		for item in self.ast.ext:
			if type(item) == c_ast.FuncDef and type(item.decl) == c_ast.Decl and item.decl.name == function_name:
				return item
		raise NoMainFunctionException

	def identify_main_loop(self):
		"""
		Identifies the main loop of the given PyCParser AST in the body of the given function and returns the loop.
		:parameter function: The function to find main loop in.
		:raise: MultipleMainLoopsException if multiple loops in the entry function are present.
				NoMainLoopException if no main loop can be identified.
		:return: The main loop of the entry function. None if there is no loop in the function body.
		:rtype: c_ast.DoWhile or c_ast.While or c_ast.For
		"""
		main_function = self.identify_function(self.main_function_name)
		loop_finder = LoopFinder()
		loop_finder.visit(main_function.body)
		loops = loop_finder.loops
		if len(loops) == 0:
			raise NoMainLoopException
		elif len(loops) > 1:
			raise MultipleMainLoopsException(loops)
		else:
			return loops[0]

	def identify_property(self):
		"""
		Identifies the verification property in the given compound block, if there is one. If there is none, returns an
		empty expression list.
		:parameter block: The compound block to identify the property in.
		:return: The verification property as an expression list, interpretable as a conjunction if there is more than
		one element.
		:rtype: c_ast.ExprList
		"""
		property_finder = PropertyFinder()
		property_finder.visit(self.identify_main_loop())
		return property_finder.properties

	def identify_typedefs(self):
		"""
		Searches for all typedefs. Returns the typedefs as a list of tuples, where the left hand side contains the
		original C types as a list of strings and the right hand side the typedef'd alias as a string.
		:param ast: The AST to find the typedef's in.
		:return: A list of tuples containing the typedefs.
		:rtype: list
		"""
		collector = TypedefCollector()
		collector.visit(self.ast)
		return collector.typedefs

	def resolve_typedefs(self, declaration: c_ast.Node, typedefs: list):
		"""
		Resolves any typedefs present in the given declaration for the given typedef list. Works in-place and applies
		itself recursively until no more typedefs are found.
		:param declaration: The declaration in which to resolve typedefs in. Type is one of c_ast.TypeDecl,
			c_ast.PtrDecl or c_ast.ArrayDecl.
		:param typedefs: A list of typedefs to check against.
		"""
		if type(declaration) == c_ast.TypeDecl:
			for typedef in typedefs:
				if type(declaration.type) == c_ast.Struct or type(declaration.type) == c_ast.Union:
					if typedef.name == declaration.type.name:
						declaration.type = copy.deepcopy(typedef.type.type)
						declaration.quals = copy.deepcopy(typedef.quals)
				elif type(declaration.type) == c_ast.IdentifierType:
					if typedef.name in declaration.type.names:
						declaration.type = copy.deepcopy(typedef.type.type)
			if type(declaration.type) == c_ast.Struct or type(declaration.type) == c_ast.Union:
				for member in declaration.type.decls:
					self.resolve_typedefs(member.type, typedefs)
		elif type(declaration) == c_ast.ArrayDecl or type(declaration) == c_ast.PtrDecl:
			self.resolve_typedefs(declaration.type, typedefs)