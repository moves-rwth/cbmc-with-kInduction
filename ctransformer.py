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

VERIFIER_NONDET_FUNCTION_NAME = "__VERIFIER_nondet_"

class NonSvCompTypeException(Exception):
	"""
	Indicates that the variable type can not be transformed into a valid SV-comp non-deterministic assignment.
	"""
	def __init__(self, variable: str):
		self.variable = variable

	def __str__(self):
		message = "Can not assign variable " + str(self.variable) + " a non-deterministic value."
		return message

class CompoundInserter(c_ast.NodeVisitor):
	"""
	Inserts the given node into the AST in a compound block once it encounters its new neighbouring node.
	"""
	def __init__(self, new_node: c_ast.Node, before_node: c_ast.Node):
		self.new_node = new_node
		self.before_node = before_node

	def visit_Compound(self, node):
		for i, (c_name, c) in enumerate(node.children()):
			if c == self.before_node:
				node.block_items.insert(i, self.new_node)
			self.visit(c)

class AggregateDeanonymizer(c_ast.NodeVisitor):
	"""
	Visits all structures and unions and adds a typename if there is none.
	"""
	def __init__(self, unavailable_identifiers: set):
		self.unavailable_identifiers = unavailable_identifiers

	def visit_Struct(self, node):
		if node.name == None:
			node.name = self.__generate_new_typename()

	def visit_Union(self, node):
		if node.name == None:
			node.name = self.__generate_new_typename()

	def __generate_new_typename(self):
		for i in range(5, sys.maxsize):
			typename = "".join(random.choices(string.ascii_letters, k=i))
			if typename not in self.unavailable_identifiers:
				break
		return typename

class IdentifierCollector(c_ast.NodeVisitor):
	def __init__(self):
		self.identifiers = set()

	def visit_TypeDecl(self, node):
		if (type(node.type) == c_ast.Struct or type(node.type) == c_ast.Union) and node.type.name is not None:
			self.identifiers.add(node.type.name)

class CTransformer:
	def __init__(self, ast: c_ast.FileAST):
		self.ast = ast

	def deanonymize_aggregates(self):
		"""
		Deanonymizes all anonymous aggregates such that they are identified by a unique name. An example of an anonymous
		aggregate: "typedef struct { int x; } s;", which will be changed to "typedef struct _s { int x; } s;" sucht that
		it can be identified by the "_s" is not anonymous anymore.
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
		:return: A block containing the havocing of the given variables and a function call for that function.
		:raise: NonSvCompTypeException in case a variable was given that can not be havoced.
		:rtype: c_ast.Compound
		"""
		# TODO
		body_items = []
		# Creates the havoc assignment for each variable.
		for declaration in declarations:
			body_items.append(self.create_havoc_assignment(declaration))
		# Bundles the havoc assignments into one compound statement.
		return c_ast.Compound(body_items)

	def create_havoc_assignment(self, declaration: c_ast.Decl, parent: c_ast.Node=None):
		"""
		Creates a havoc assignment block for the variable declared in the given declaration.
		:param declaration: The declaration of the variable to havoc.
		:return: A block containing all havoc assignments for that variable.
		:parent: A parent node for aggregates to allow for access of the children. Either c_ast.StructRef,
			c_ast.ArrayRef or c_ast.UnaryOp with op="*".
		:rtype: c_ast.Compound
		"""
		body_items = []
		# First, registers itself into the parent struct, if there is one.
		if type(parent) == c_ast.StructRef and parent.field is None:
			parent.field = c_ast.ID(declaration.name)
		# Checks for five main cases: We have a basic identifier, a struct, a union, an array or a pointer.
		if type(declaration.type) == c_ast.TypeDecl:
			# CASE STRUCT
			if type(declaration.type.type) == c_ast.Struct:
				for member in declaration.type.type.decls:
					if parent is None:
						new_parent = c_ast.StructRef(c_ast.ID(declaration.name), ".", None)
					else:
						new_parent = c_ast.StructRef(parent, ".", None)
					body_items.append(self.create_havoc_assignment(member, new_parent))
			# CASE UNION
			elif type(declaration.type.type) == c_ast.Union and len(declaration.type.type.decls) > 0:
				if parent is None:
					new_parent = c_ast.StructRef(c_ast.ID(declaration.name), ".", None)
				else:
					new_parent = c_ast.StructRef(parent, ".", None)
				body_items.append(self.create_havoc_assignment(declaration.type.type.decls[0], new_parent))
			# CASE BASIC IDENTIFIER
			elif type(declaration.type.type) == c_ast.IdentifierType:
				havoc_function = VERIFIER_NONDET_FUNCTION_NAME + self.get_svcomp_type(declaration.type.type.names)
				rvalue = self.create_function_call(havoc_function)
				if parent is None:
					lvalue = c_ast.ID(declaration.name)
				else:
					lvalue = parent
				havoc_variable = c_ast.Assignment("=", lvalue, rvalue)
				body_items.append(havoc_variable)
		# CASE ARRAY
		elif type(declaration.type) == c_ast.ArrayDecl:
			modified_declaration = copy.deepcopy(declaration)
			modified_declaration.type = modified_declaration.type.type
			if type(declaration.type.dim) == c_ast.Constant and declaration.type.dim.type == "int":
				for i in range(int(declaration.type.dim.value)):
					if parent is None:
						new_parent = c_ast.ID(declaration.name)
					else:
						new_parent = parent
					body_items.append(
						self.create_havoc_assignment(
							modified_declaration,
							c_ast.ArrayRef(new_parent, c_ast.Constant("int", str(i)))
						)
					)
			else:
				print("WARNING: Non-constant array encountered!") # TODO?
		# CASE POINTER
		elif type(declaration.type) == c_ast.PtrDecl:
			if type(declaration.type.type) == c_ast.TypeDecl and \
					type(declaration.type.type.type) == c_ast.IdentifierType and \
					"void" in declaration.type.type.type.names:
				havoc_function = VERIFIER_NONDET_FUNCTION_NAME + "pointer"
				rvalue = self.create_function_call(havoc_function)
				if parent is None:
					lvalue = c_ast.ID(declaration.name)
				else:
					lvalue = parent
				havoc_variable = c_ast.Assignment("=", lvalue, rvalue)
				body_items.append(havoc_variable)
			else:
				modified_declaration = copy.deepcopy(declaration)
				modified_declaration.type = modified_declaration.type.type
				if parent is None:
					new_parent = c_ast.ID(declaration.name)
				else:
					new_parent = parent
				body_items.append(
						self.create_havoc_assignment(
								modified_declaration,
								c_ast.UnaryOp("*", new_parent)
						)
				)
		# Bundles the havoc assignments into one compound statement.
		return c_ast.Compound(body_items)

	def get_svcomp_type(self, type_names: list):
		"""
		Searches for the corresponding SV comp type for the given list of C types.
		:param type_names: A list of strings containing the C types, e.g. ["unsigned", "int"].
		:raise: NonSvCompTypeException in case the SV comp type could not be identified.
		:return: A string representing the SV comp type, e.g. "uint".
		"""
		svcomp_type = None
		type_names = set(type_names)
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

	def create_negated_expression(self, expression: c_ast.ExprList):
		"""
		Creates a negated expression from the given expression.
		:param expression: The expression to negate.
		:return: An expression that represents the negation.
		:rtype: c_ast.ExprList
		"""
		# TODO
		return None

	def create_function_call(self, name: str, parameters: c_ast.ExprList = c_ast.ExprList([])):
		"""
		Creates a function call containing the given parameters. Does not change the AST.
		:param name: The name of the function.
		:param parameters: The parameters of the call. Defaults to no parameters.
		:return: A function call.
		:rtype: c_ast.FuncCall
		"""
		return c_ast.FuncCall(c_ast.ID(name), parameters)

	def insert(self, node: c_ast.Node, before: c_ast.Node):
		"""
		Inserts the given block at the given position into the the AST. The position determined by the before parameter,
		which leads to an insertion right before the node in the compound statement. If the node is not present, the
		insertion takes place after the last element. Note that the node has to be contained in a compound statement
		(i.e. block) where insertion is possible. Otherwise, the AST remains unchanged.
		Operates in-place on the AST.
		:param node: The node to insert.
		:param before: The node before which to insert.
		"""
		CompoundInserter(node, before).visit(self.ast)