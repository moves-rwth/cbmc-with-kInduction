"""
Performs various (static) C code analyses using pycparser.
Currently supported analyses:
	- Variable occurrence:
		Lists all variables that occur in a function.
	- Variable modification:
		Lists all variables that are modified in a function.
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
from pycparser import c_ast
from pycparserext.ext_c_parser import FuncDeclExt, TypeDeclExt
from pycparserext.ext_c_generator import GnuCGenerator

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
			message += str(i) + ")\n" + GnuCGenerator().visit(loop) + "\n"
		return message

class NoMainLoopException(Exception):
	"""
	Indicates that no main loop of the identified main function name could be found.
	"""
	def __init__(self, main_function_name: str="main"):
		self.main_function_name = main_function_name

	def __str__(self):
		return "Can not find a loop within function " + self.main_function_name + "."

class NoSuchFunctionException(Exception):
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
	Finds the verification property in the AST node and puts it into its properties member.
	Searches for all compound blocks that contains a function call with the verifier error function and assert
	statements. It then assembles the property via the surrounding if statements or the arguments of the assert.
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
			i = -1
			while -i <= len(self.parents) and type(self.parents[i]) == c_ast.Compound and \
					type(self.parents[i - 1]) == c_ast.If:
				if type(self.parents[i - 1].cond) == c_ast.ExprList:
					self.properties.exprs += self.parents[i - 1].cond.exprs
				else:
					self.properties.exprs.append(self.parents[i - 1].cond)
				i -= 2
		# Case assert was used.
		elif node.name.name == ASSERT_FUNCTION_NAME:
			if type(node.args) == c_ast.ExprList:
				self.properties.exprs += node.args.exprs
			else:
				self.properties.exprs.append(node.args)

class PropertyStatementFinder(c_ast.NodeVisitor):
	"""
	Finds the statement surrounding the verification property in the AST node and puts it into its property member.
	"""
	def __init__(self):
		self.statement = None
		self.parents = []

	def generic_visit(self, node):
		self.parents.append(node)
		for c_name, c in node.children():
			self.visit(c)
		self.parents = self.parents[:-1]

	def visit_FuncCall(self, node):
		# Case __VERIFIER_error was used.
		if node.name.name == VERIFIER_ERROR_FUNCTION_NAME:
			i = -1
			while -i <= len(self.parents) and type(self.parents[i]) == c_ast.Compound and \
					type(self.parents[i - 1]) == c_ast.If:
				self.statement = self.parents[i - 1]
				i -= 2
		# Case assert was used.
		elif node.name.name == ASSERT_FUNCTION_NAME:
			self.statement = node

class TypedefAndStructsCollector(c_ast.NodeVisitor):
	"""
	Collects all typedefs and structs.
	"""
	def __init__(self):
		self.typedefs = []

	def generic_visit(self, node):
		if type(node) != FuncDeclExt and type(node) != TypeDeclExt:
			for c in node:
				self.visit(c)

	def visit_Typedef(self, node):
		self.typedefs.append(node)

	def visit_Decl(self, node):
		# Struct tags can also be used as a type elsewhere.
		if hasattr(node, "type") and type(node.type) is c_ast.Struct:
			self.typedefs.append(node.type)

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
		if type(node.type) != c_ast.FuncDecl and type(node.type) != FuncDeclExt and node.name and len(self.parents) > 0\
				and type(self.parents[-1]) is not c_ast.Typedef:
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

class AggregateRemover(c_ast.NodeVisitor):
	"""
	Visits all aggregates in compounds and ASTs and removes matching ones.
	"""
	def __init__(self, aggregate):
		self.aggregate = aggregate

	def visit_Compound(self, node):
		for item in node.block_items:
			if hasattr(item, "type") and item.type == self.aggregate:
				node.block_items.remove(item)

	def visit_FileAST(self, node):
		for item in node.ext:
			if hasattr(item, "type") and item.type == self.aggregate:
				node.ext.remove(item)

########################################################################################################################
# Analyzer                                                                                                             #
########################################################################################################################

class CAnalyzer:
	def __init__(self, ast: c_ast.FileAST, main_function_name: str="main"):
		self.ast = ast
		self.main_function_name = main_function_name
		self.declarations = None

	def identify_declarations(self, function: c_ast.FuncDef):
		"""
		Collects a list of all variables used in the function-scope - local and global. Each variable is represented by
		its declaration, in which any typedefs are already completely resolved.
		:param function: The functions to identify variables in.
		:return: A list of declarations, one for each variable.
		:rtype: list of c_ast.Decl
		"""
		if not self.declarations:
			declarations = []
			generator = GnuCGenerator()
			typedefs = self.identify_typedefs()
			collector = DeclarationCollector()
			collector.visit(self.ast)
			for declaration, scope in collector.declarations:
				# Declaration can be valid in either the function parameters, the function body or in the global scope.
				if scope == function or scope == function.body or scope == self.ast:
					declarations.append(declaration)
					# Fixed point iteration until all typedefs are resolved.
					old_declaration = copy.deepcopy(declaration)
					first = True
					while str(generator.visit(old_declaration)) != str(generator.visit(declaration)) or first:
						old_declaration = copy.deepcopy(declaration)
						self.resolve_typedefs(declaration.type, typedefs)
						first = False
			# Cleaning up the resolving process: Removing tag-only struct types (as they are resolved now).
			for typedef in typedefs:
				print(typedef)
				if type(typedef) is c_ast.Struct:
					typedef.name = None
			self.declarations = declarations
		return self.declarations

	def is_variable_of_declaration_modified(self, declaration: c_ast.Decl, block: c_ast.Compound):
		"""
		Returns true iff the variable of the declaration is modified in the given compound.
		:param declaration: The declaration of the variable to check for.
		:param block: The block to which the modifications should be local.
		:return: True or false.
		"""
		if "const" in declaration.quals and type(declaration.type) is not c_ast.PtrDecl:
			# Constant variables can not be modified. Constant pointers can be modified!
			return False
		else:
			# We have a non-aggregate basic C type. We can now check for variable occurrences in LHS of assignments.
			# TODO Can we actually make any assumptions about that, as pointer aliasing exists?
			# int x = 1;
			# int* y = &x;
			# *y = 2;
			# Hence, we need to check if occurring on LHS or the address is somehow taken from it. Then we return True.
			# Some static analysis algorithms may be useful here. What does Frama-C implement for this?
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
		Identifies the function with the given identifier name in the AST.
		:param function_name: The identifier of the function to search for.
		:raise: NoSuchFunctionException if no function of the given function name could be identified.
		:return: The main function.
		:rtype: c_ast.FuncDef
		"""
		for item in self.ast.ext:
			if type(item) == c_ast.FuncDef and type(item.decl) == c_ast.Decl and item.decl.name == function_name:
				return item
		raise NoSuchFunctionException(function_name)

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
		# Checks if exactly one loop is present.
		if len(loops) == 0:
			raise NoMainLoopException
		elif len(loops) > 1:
			raise MultipleMainLoopsException(loops)
		else:
			return loops[0]

	def identify_property(self):
		"""
		Identifies the verification property in the given compound block, if there is one. If there none, returns an
		empty expression list.
		:parameter block: The compound block to identify the property in.
		:return: The verification property as an expression list, interpretable as a conjunction if there is more than
		one element.
		:rtype: c_ast.ExprList
		"""
		property_finder = PropertyFinder()
		property_finder.visit(self.identify_main_loop())
		return property_finder.properties

	def identify_property_statement(self):
		"""
		Identifies the statement that contains the verification property, i.e. either an if statement or an assert
		function call.
		:return: The statement responsible for the property check.
		:rtype: c_ast.Node
		"""
		property_statement_finder = PropertyStatementFinder()
		property_statement_finder.visit(self.identify_main_loop())
		return property_statement_finder.statement

	def identify_typedefs(self):
		"""
		Searches for all typedefs. Returns the typedefs as a list of tuples, where the left hand side contains the
		original C types as a list of strings and the right hand side the typedef'd alias as a string.
		:param ast: The AST to find the typedef's in.
		:return: A list of tuples containing the typedefs.
		:rtype: list
		"""
		collector = TypedefAndStructsCollector()
		collector.visit(self.ast)
		return collector.typedefs

	def resolve_typedefs(self, declaration: c_ast.Node, typedefs: list):
		"""
		Resolves any typedefs present in the given declaration for the given typedef list. Works in-place on the AST and
		applies itself recursively until no more typedefs are found.
		:param declaration: The declaration in which to resolve typedefs in. Type is one of c_ast.TypeDecl,
			c_ast.PtrDecl or c_ast.ArrayDecl.
		:param typedefs: A list of typedefs to check against.
		"""
		# Three cases to be distinguished: Type, array or pointer declaration. For arrays, pointers, the type
		# resolving is just applied recursively. For a type declaration, all typedefs are first resolved, and in case we
		# have a aggregate type (struct or union), the resolving is again applied recursively to their members.
		# Keeps track of non-resolved typedefs for recursive application of this function. We do not want already
		# resolved typedefs to be re-applied again.
		unresolved_typedefs = copy.copy(typedefs)
		if type(declaration) == c_ast.TypeDecl:
			for typedef in typedefs:
				if type(typedef) is c_ast.Typedef:
					if type(declaration.type) == c_ast.Struct or type(declaration.type) == c_ast.Union:
						if typedef.name == declaration.type.name:
							declaration.type = copy.deepcopy(typedef.type.type)
							declaration.quals = copy.deepcopy(typedef.quals)
					elif type(declaration.type) == c_ast.IdentifierType:
						if typedef.name in declaration.type.names:
							declaration.type = copy.deepcopy(typedef.type.type)
				elif type(typedef) is c_ast.Struct and type(declaration.type) == c_ast.Struct:
					if typedef.name == declaration.type.name:
						declaration.type = copy.deepcopy(typedef)
						unresolved_typedefs.remove(typedef)
			if type(declaration.type) == c_ast.Struct or type(declaration.type) == c_ast.Union:
				if declaration.type.decls:
					for member in declaration.type.decls:
						self.resolve_typedefs(member.type, typedefs)
		elif type(declaration) == c_ast.ArrayDecl or type(declaration) == c_ast.PtrDecl:
			return self.resolve_typedefs(declaration.type, typedefs)