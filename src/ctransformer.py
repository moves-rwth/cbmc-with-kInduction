"""
Performs C code transformations using pycparser. All the transformations are applied in-place on the AST.
Currently supported code transformations:
	- Function call creation
		Creates a statement representing a function call.
	- Havoc block creation
		Creates a block where every variable is assigned a non-deterministic value.
	- Insertion into compound statements
		Inserts any node into a compound statement at a user-specified position.
"""
import copy
import sys
import random
import string
from pycparser import c_ast
from pycparserext.ext_c_parser import GnuCParser
from pycparserext.ext_c_generator import GnuCGenerator
from pycparserext.ext_c_parser import FuncDeclExt, TypeDeclExt

VERIFIER_NONDET_FUNCTION_NAME = "__VERIFIER_nondet_"
VERIFIER_ERROR_FUNCTION_NAME  = "__VERIFIER_error"
ASSERT_FUNCTION_NAME          = "assert"

########################################################################################################################
# Exceptions                                                                                                           #
########################################################################################################################

class NonSvCompTypeException(Exception):
	"""
	Indicates that the variable type can not be transformed into a valid SV-comp non-deterministic assignment.
	"""
	def __init__(self, variable: str):
		self.variable = variable

	def __str__(self):
		message = "Can not assign variable " + str(self.variable) + " a non-deterministic value."
		return message

########################################################################################################################
# AST Node Visitors                                                                                                    #
########################################################################################################################

class CompoundInserter(c_ast.NodeVisitor):
	"""
	Inserts the given node into the AST in a compound block or AST once it encounters its new neighbouring node.
	"""
	def __init__(self, new_node: c_ast.Node, before_node: c_ast.Node, after_node: c_ast.Node):
		self.new_node    = new_node
		self.before_node = before_node
		self.after_node  = after_node

	def generic_visit(self, node):
		if type(node) != FuncDeclExt and type(node) != TypeDeclExt:
			for c in node:
				self.visit(c)

	def visit_FileAST(self, node):
		for i, (c_name, c) in enumerate(node.children()):
			if self.before_node and c == self.before_node:
				node.ext.insert(i, self.new_node)
			elif self.after_node and c == self.after_node:
				node.ext.insert(i + 1, self.new_node)
			self.visit(c)

	def visit_Compound(self, node):
		for i, (c_name, c) in enumerate(node.children()):
			if self.before_node and c == self.before_node:
				node.block_items.insert(i, self.new_node)
			elif self.after_node and c == self.after_node:
				node.block_items.insert(i + 1, self.new_node)
			self.visit(c)

class AggregateDeanonymizer(c_ast.NodeVisitor):
	"""
	Visits all structures and unions and adds a typename if there is none.
	"""
	def __init__(self, unavailable_identifiers: set):
		self.unavailable_identifiers = unavailable_identifiers

	def generic_visit(self, node):
		if type(node) != FuncDeclExt and type(node) != TypeDeclExt:
			for c in node:
				self.visit(c)

	def visit_Struct(self, node):
		if node.name == None:
			node.name = self.__generate_new_typename()

	def visit_Union(self, node):
		if node.name == None:
			node.name = self.__generate_new_typename()

	def __generate_new_typename(self):
		typename = ""
		for i in range(5, sys.maxsize):
			typename = "".join(random.choices(string.ascii_letters, k=i))
			if typename not in self.unavailable_identifiers:
				break
		return typename

class IdentifierCollector(c_ast.NodeVisitor):
	def __init__(self):
		self.identifiers = set()

	def generic_visit(self, node):
		if type(node) != FuncDeclExt and type(node) != TypeDeclExt:
			for c in node:
				self.visit(c)

	def visit_TypeDecl(self, node):
		if (type(node.type) == c_ast.Struct or type(node.type) == c_ast.Union) and node.type.name is not None:
			self.identifiers.add(node.type.name)

class PropertyReplacer(c_ast.NodeVisitor):
	"""
	Finds the verification property in the AST node and replaces it with the given one.
	Searches for all compound blocks that contains a function call with the verifier error function and assert
	statements. It then assembles the property via the surrounding if statement or the arguments of the assert.
	Stores the properties as an ExprList whose elements should be understood as a conjunct.
	"""
	def __init__(self, new_property: c_ast.ExprList):
		self.new_property = new_property
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
				self.parents[-2].cond = self.new_property
		# Case assert was used.
		elif node.name.name == ASSERT_FUNCTION_NAME:
				node.args = self.new_property

class DeclarationReplacer(c_ast.NodeVisitor):
	"""
	Replaces the initializer of the given declaration with the new constant integer value.
	"""
	def __init__(self, declaration: c_ast.Decl, new_value: int):
		self.declaration = declaration
		self.new_value = new_value

	def visit_Decl(self, node):
		if node.name == self.declaration.name:
			node.init = c_ast.Constant("int", str(self.new_value))

########################################################################################################################
# Transformer                                                                                                          #
########################################################################################################################

class CTransformer:
	def __init__(self, ast: c_ast.FileAST):
		self.ast = ast

	def deanonymize_aggregates(self):
		"""
		Deanonymizes all anonymous aggregates such that they are identified by a unique name. An example of an anonymous
		aggregate: "typedef struct { int x; } s;", which will be changed to "typedef struct _s { int x; } s;" such that
		it can be identified by the "_s". Hence, it is not anonymous anymore after this function call.
		"""
		collector = IdentifierCollector()
		collector.visit(self.ast)
		deanonymizer = AggregateDeanonymizer(collector.identifiers)
		deanonymizer.visit(self.ast)

	def create_havoc_block(self, declarations: list):
		"""
		Creates a block containing the non-deterministic assignments to the variables in the given declarations.
		Respects the types of those variables, e.g. an integer will be assigned __VERIFIER_nondet_int(). Structs and
		arrays will be unrolled and their members will be havoced.
		:param variables: A list of c_ast.Decl.
		:return: First entry: A set of strings containing the employed non-deterministic assignment SV comp function
			names, e.g. "__VERIFIER_nondet_int". Second entry: A block containing all havoc assignments for that
			variable.
		:raise: NonSvCompTypeException in case a variable was given that can not be havoced.
		:rtype: set of str, c_ast.Compound
		"""
		body_items = []
		svcomp_havoc_functions = set()
		# Creates the havoc assignment for each variable.
		for declaration in declarations:
			rec_svcomp_havoc_functions, havoc_block = self.create_havoc_assignment(declaration)
			body_items.append(havoc_block)
			svcomp_havoc_functions = svcomp_havoc_functions.union(rec_svcomp_havoc_functions)
		# Bundles the havoc assignments into one compound statement.
		return svcomp_havoc_functions, c_ast.Compound(body_items)

	def create_havoc_assignment(self, declaration: c_ast.Decl, parent: c_ast.Node=None):
		"""
		Creates a havoc assignment block for the variable declared in the given declaration.
		:param declaration: The declaration of the variable to havoc.
		:return: First entry: A set of strings containing the employed non-deterministic assignment SV comp function
			names, e.g. "__VERIFIER_nondet_int". Second entry: A block containing all havoc assignments for that
			variable.
		:parent: A parent node for aggregates to allow for access of the children. Either c_ast.StructRef,
			c_ast.ArrayRef or c_ast.UnaryOp with op="*".
		:rtype: set of str, c_ast.Compound
		"""
		# Here be dragons. Most likely contains some bugs.
		# TODO Should be tested thoroughly.
		body_items = []
		svcomp_havoc_functions = set()
		# First, registers itself into the parent struct, if there is one.
		if type(parent) == c_ast.StructRef and parent.field is None:
			parent.field = c_ast.ID(declaration.name)
		# Checks for five main cases: We have a basic identifier, a struct, a union, an array or a pointer.
		# If a compound type is encountered, this function is called recursively on the child declaration(s).
		if type(declaration.type) == c_ast.TypeDecl:
			# CASE STRUCT
			if type(declaration.type.type) == c_ast.Struct:
				# Iterates over every struct member and creates a havoc block for this. Useful for nested structs.
				if declaration.type.type.decls:
					for member in declaration.type.type.decls:
						if parent is None:
							new_parent = c_ast.StructRef(c_ast.ID(declaration.name), ".", None)
						else:
							new_parent = c_ast.StructRef(parent, ".", None)
						rec_svcomp_havoc_funcs, rec_havoc_block = self.create_havoc_assignment(member, new_parent)
						body_items.append(rec_havoc_block)
						svcomp_havoc_functions = svcomp_havoc_functions.union(rec_svcomp_havoc_funcs)
			# CASE UNION
			elif type(declaration.type.type) == c_ast.Union and len(declaration.type.type.decls) > 0:
				# For a union, we just havoc the very first member.
				if parent is None:
					new_parent = c_ast.StructRef(c_ast.ID(declaration.name), ".", None)
				else:
					new_parent = c_ast.StructRef(parent, ".", None)
				rec_svcomp_havoc_funcs, rec_havoc_block = self.create_havoc_assignment(declaration.type.type.decls[0],
																					   new_parent)
				body_items.append(rec_havoc_block)
				svcomp_havoc_functions = svcomp_havoc_functions.union(rec_svcomp_havoc_funcs)
			# CASE BASIC IDENTIFIER
			elif type(declaration.type.type) == c_ast.IdentifierType:
				# Base case of the recursion.
				havoc_function = VERIFIER_NONDET_FUNCTION_NAME + self.get_svcomp_type(declaration.type.type.names)
				rvalue = self.create_function_call(havoc_function)
				if parent is None:
					lvalue = c_ast.ID(declaration.name)
				else:
					lvalue = parent
				havoc_variable = c_ast.Assignment("=", lvalue, rvalue)
				body_items.append(havoc_variable)
				svcomp_havoc_functions.add(havoc_function)
		# CASE ARRAY
		elif type(declaration.type) == c_ast.ArrayDecl:
			modified_declaration = copy.deepcopy(declaration)
			modified_declaration.type = modified_declaration.type.type
			if type(declaration.type.dim) == c_ast.Constant and declaration.type.dim.type == "int":
				# Iterates over every member of the array (Thus, the size has to be constant).
				for i in range(int(declaration.type.dim.value)):
					if parent is None:
						new_parent = c_ast.ID(declaration.name)
					else:
						new_parent = parent
					rec_svcomp_havoc_funcs, rec_havoc_block = self.create_havoc_assignment(
							modified_declaration,
							c_ast.ArrayRef(new_parent, c_ast.Constant("int", str(i)))
						)
					body_items.append(rec_havoc_block)
					svcomp_havoc_functions = svcomp_havoc_functions.union(rec_svcomp_havoc_funcs)
			else:
				print("WARNING: Non-constant array encountered!") # TODO? (Can be done by just assigning a pointer.)
		# CASE POINTER
		elif type(declaration.type) == c_ast.PtrDecl:
			if type(declaration.type.type) == c_ast.TypeDecl and \
					type(declaration.type.type.type) == c_ast.IdentifierType and \
					("const" not in declaration.type.quals or "void" in declaration.type.type.type.names):
				# Base case of the recursion. Only entered if we can not dereference the pointer due to either an
				# unknown type (void pointer) or a constant memory location behind the pointer.
				havoc_function = VERIFIER_NONDET_FUNCTION_NAME + "pointer"
				svcomp_havoc_functions.add(havoc_function)
				rvalue = self.create_function_call(havoc_function)
				if parent is None:
					lvalue = c_ast.ID(declaration.name)
				else:
					lvalue = parent
				havoc_variable = c_ast.Assignment("=", lvalue, rvalue)
				body_items.append(havoc_variable)
			else:
				# We can dereference the pointer: Does so and creates a havoc statement for the type behind the pointer.
				modified_declaration = copy.deepcopy(declaration)
				modified_declaration.type = modified_declaration.type.type
				if parent is None:
					new_parent = c_ast.ID(declaration.name)
				else:
					new_parent = parent
				rec_svcomp_havoc_funcs, rec_havoc_block =  self.create_havoc_assignment(modified_declaration,
																						c_ast.UnaryOp("*", new_parent))
				body_items.append(rec_havoc_block)
				svcomp_havoc_functions = svcomp_havoc_functions.union(rec_svcomp_havoc_funcs)
		# Bundles the havoc assignments into one compound statement.
		if len(body_items) == 0:
			sys.stderr.write("WARNING: Could not havoc variable of declaration " + GnuCGenerator().visit(declaration) +
							 "\n")
		return svcomp_havoc_functions, c_ast.Compound(body_items)

	def get_svcomp_type(self, type_names: list):
		"""
		Searches for the corresponding SV comp type for the given list of C types.
		:param type_names: A list of strings containing the C types, e.g. ["unsigned", "int"].
		:raise: NonSvCompTypeException in case the SV comp type could not be identified.
		:return: A string representing the SV comp type, e.g. "uint".
		"""
		svcomp_type = None
		type_names = set(type_names)
		# Implements the translation from C types to SV comp types.
		if "bool" in type_names:
			svcomp_type = "bool"
		elif "float" in type_names:
			svcomp_type = "float"
		elif "double" in type_names:
			svcomp_type = "double"
		elif "loff_t" in type_names:
			svcomp_type = "loff_t"
		elif "pchar" in type_names:
			svcomp_type = "pchar"
		elif "pthread_t" in type_names:
			svcomp_type = "pthread_t"
		elif "sector_t" in type_names:
			svcomp_type = "sector_t"
		elif "size_t" in type_names:
			svcomp_type = "size_t"
		elif "u32" in type_names:
			svcomp_type = "u32"
		elif "char" in type_names:
			svcomp_type = "char"
		elif "short" in type_names:
			svcomp_type = "short"
		elif "long" in type_names:
			svcomp_type = "long"
		elif "int" in type_names:
			svcomp_type = "int"
		if "unsigned" in type_names:
			svcomp_type = "u" + svcomp_type
		if not svcomp_type:
			print(type_names)
			raise NonSvCompTypeException(" ".join(type_names))
		return svcomp_type

	def get_c_type(self, svcomp_type: str):
		"""
		Searches for the corresponding list of C types for the given SV comp type.
		:param type_names: A string representing the SV comp type, e.g. "uint".
		:raise: NonSvTypeException in case the SV comp type could not be identified.
		:return: A list of strings containing the C types, e.g. ["unsigned", "int"].
		"""
		type_names = set()
		# Implements the translation from C types to SV comp types.
		if svcomp_type == "bool":
			type_names.add("bool")
		elif svcomp_type == "float":
			type_names.add("float")
		elif svcomp_type == "double":
			type_names.add("double")
		elif svcomp_type == "loff_t":
			type_names.add("loff_t")
		elif svcomp_type == "pchar":
			type_names.add("pchar")
		elif svcomp_type == "pthread_t":
			type_names.add("pthread_t")
		elif svcomp_type == "sector_t":
			type_names.add("sector_t")
		elif svcomp_type == "size_t":
			type_names.add("size_t")
		elif svcomp_type == "u32":
			type_names.add("u32")
		elif svcomp_type == "char":
			type_names.add("char")
		elif svcomp_type == "short":
			type_names.add("short")
		elif svcomp_type == "long":
			type_names.add("long")
		elif svcomp_type == "int":
			type_names.add("int")
		elif svcomp_type == "pointer":
			type_names.add("void")
			type_names.add("*")
		if svcomp_type.startswith("u"):
			type_names.add("unsigned")
		if not svcomp_type:
			print(type_names)
			raise NonSvCompTypeException(svcomp_type)
		return type_names

	def add_to_expression(self, expression: c_ast.Node, operator: str, addition: c_ast.ExprList=None):
		"""
		Adds the additional expression to the given expression, concatenated with the given operator. If the additional
		expression is None, the operator is assumed to be unary.
		:param expression: The expression to add to.
		:param operator: An operator on expression, e.g. "&&" or "!".
		:param addition: The expression to add.
		:return: The merged expression.
		:rtype: c_ast.ExprList
		"""
		expressions = []
		if type(expression) is c_ast.ExprList:
			for expr in expression.exprs:
				if addition is None:
					expressions.append(c_ast.UnaryOp(operator, copy.deepcopy(expr)))
				else:
					expressions.append(c_ast.BinaryOp(operator, copy.deepcopy(expr), addition))
		else:
			if addition is None:
				expressions.append(c_ast.UnaryOp(operator, copy.deepcopy(expression)))
			else:
				expressions.append(c_ast.BinaryOp(operator, copy.deepcopy(expression), addition))
		return c_ast.ExprList(expressions)

	def join_expression_list(self, operator: str, expressions: c_ast.ExprList):
		"""
		Joins every element in the list with given the binary operator, e.g. ["a", "b"] and "&&" -> "a && b".
		:param operator: A binary operator string for C.
		:param expressions: The expression list.
		:return: A joined expression where each element is copied.
		:rtype: c_ast.Node
		"""
		if len(expressions.exprs) > 0:
			joined = copy.deepcopy(expressions.exprs[0])
			for property in expressions.exprs[1:]:
				joined = c_ast.BinaryOp(operator, joined, copy.deepcopy(property))
			return joined
		return copy.deepcopy(expressions)

	def replace_property(self, expression: c_ast.ExprList):
		"""
		Replaces the property in the main loop with the given expression.
		:param expression: The new expression.
		"""
		PropertyReplacer(expression).visit(self.ast)

	def replace_initial_value(self, declaration: str, scope: c_ast.Node, new_value: int):
		"""
		Replaces the initial value of the given declaration with the new value.
		:param declaration: The declaration in which the initial value is to be replaced, as a string.
		:param scope: Where to look for the declaration.
		:param new_value: The new intial value.
		"""
		declaration_node = self.from_code(declaration)
		if len(declaration_node.block_items) > 0:
			DeclarationReplacer(declaration_node.block_items[0], new_value).visit(scope)

	def create_function_call(self, name: str, parameters: c_ast.Node = c_ast.ExprList([])):
		"""
		Creates a function call containing the given parameters. Does not change the AST.
		:param name: The name of the function.
		:param parameters: The parameters of the call. Defaults to no parameters.
		:return: A function call.
		:rtype: c_ast.FuncCall
		"""
		return c_ast.FuncCall(c_ast.ID(name), parameters)

	def create_svcomp_function_declaration(self, name: str):
		"""
		Creates a declaration for the given SV comp function name, e.g. "__VERIFIER_nondet_int".
		:param name: The SV comp function name.
		:return: c_ast.Decl
		"""
		return_type = self.get_c_type(name.replace("__VERIFIER_nondet_", ""))
		# Need to handle pointers separately.
		if "*" in return_type:
			return_type.remove("*")
			return_code = c_ast.PtrDecl([], c_ast.TypeDecl(name, [], c_ast.IdentifierType(list(return_type))))
		else:
			return_code = c_ast.TypeDecl(name, [], c_ast.IdentifierType(return_type))
		return c_ast.Decl(name, [], ["extern"], [], c_ast.FuncDecl(None, return_code), None, None)

	def insert(self, node: c_ast.Node, before: c_ast.Node=None, after: c_ast.Node=None):
		"""
		Inserts the given block at the given position into the the AST. The position determined by the before/after
		parameter, which leads to an insertion right before the node in the compound statement. Note that the node has
		to be contained in a compound statement (i.e. block) where insertion is possible. Otherwise, the AST remains
		unchanged. Operates in-place on the AST.
		:param node: The node to insert.
		:param before: The node before which to insert. If None, checks the after node for position.
		:param before: The node after which to insert. If None, checks the before node for position.
		"""
		CompoundInserter(node, before, after).visit(self.ast)

	def from_code(self, code: str, parser=None):
		"""
		Takes (partial) C code as a string and returns the AST node that belongs to it. Wraps everything in a block.
		Note: This function may not understand all code strings and shall be used with care. But, it understands
		everything that is either directly valid C code, or partial code that can be put into a function body or as a
		condition block (e.g. if and while conditions).
		:param code: The C code.
		:param parser: If you have already created a parser, you can hint it here. No need to create multiple parsers.
		:return: The corresponding AST compound.
		:rtype: c_ast.Compound or None
		"""
		# TODO this is a bit hacky, trying to wrap the code s.t. it is hopefully valid to allow for partial parsing.
		if not parser: parser = GnuCParser()
		try:
			ast = parser.parse(code)
			return c_ast.Compound(ast.ext)
		except:
			try:
				wrapped_code = "void f() { " + code + " }"
				ast = parser.parse(wrapped_code)
				return ast.ext[0].body
			except:
				try:
					wrapped_code = "void f() { if(" + code + ") {} }"
					ast = parser.parse(wrapped_code)
					return c_ast.Compound([ast.ext[0].body.block_items[0].cond])
				except Exception as e:
					print(e)
					return None