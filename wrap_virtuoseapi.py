#! /usr/bin/env python

# Based slightly on:
#-----------------------------------------------------------------
# pycparser: func_defs.py
#
# Using pycparser for printing out all the functions defined in a
# C file.
#
# This is a simple example of traversing the AST generated by
# pycparser.
#
# Copyright (C) 2008-2009, Eli Bendersky
# License: LGPL
#-----------------------------------------------------------------
import sys
import os
import string

from pycparser import c_parser, c_ast, parse_file

defaultapifilenames = ['virtuoseAPI.h', 'VirtuoseAPI.h']

structtype = ["VirtContext"]
apicallqualifier = ""; #"_VAPI::"
manuallywrapped = ("virtOpen", "virtClose", "virtGetErrorMessage")

classname = "Virtuose"
argTrans = {	'fichier':		'fh',
										'intensite':	'intensity',
										'clic_gauche':'left_click',
										'clic_droit':	'right_click',
										'btn_gauche': 'left_btn',
										'btn_droit':	'right_btn',
										'btn_milieu':	'middle_btn',
										'actif':			'active',		# questionable
										'axe':				'axis',
										'decalage':		'shift'
									}


bpfilename = "vpp-boilerplate.h"
classmarker = "/* CLASS BODY GOES HERE */"
implmarker = "/* IMPLEMENTATION BODY GOES HERE */"
includeplaceholder = "UPSTREAM_INCLUDE_FILENAME"

virtcontextmember = "_vc"


defaultoutputfilename = 'vpp.h'

translateArg = lambda x: argTrans.get(x, x)

class TypeVisitor(c_ast.NodeVisitor):
	def __init__(self):
		self.type = []
		self.name = None
		print "-------------------------"

	def visit_PtrDecl(self, node):
		# recurse first
		self.generic_visit(node)

		# add a pointer symbol only if this isn't a function pointer
		if not "(*" in self.type:
			self.type.append('*')
		print "after visit_PtrDecl:", self.type

	def visit_IdentifierType(self, node):
		# recurse first
		self.generic_visit(node)

		# add typenames
		self.type.extend(node.names)
		print "after visit_IdentifierType:", self.type

	def visit_FuncDecl(self, node):
		# for function pointers

		# visit the return type
		self.visit(node.type)

		# add the function pointer name part
		self.type.append("(*")
		self.type.append(None) # indicating where to put the name
		self.type.append(")")

		# Must independently parse the args with new TypeVisitors
		args = []
		for arg in node.args.children():
			fullarg = TypeVisitor()
			fullarg.visit(arg)
			print "Argument:", fullarg.getFullType()
			args.append(" ".join(fullarg.getFullType()))

		# Append it, wrapped in parens
		self.type.append("(" + ", ".join(args) + ")")

		print "after visit_FuncDecl:", self.type

	def visit_TypeDecl(self, node):
		if node.declname is not None:
			self.name = translateArg(node.declname)
		self.generic_visit(node)
		print "after visit_TypeDecl:", self.type

	def visit_Decl(self, node):
		if node.name is not None:
			self.name = translateArg(node.name)
		self.generic_visit(node)
		print "after visit_Decl:", self.type

	def getTypeOnly(self):
		return self.type

	def getNameOnly(self):
		return self.name

	def getFullType(self):
		ret = []
		usedName = False
		for elem in self.type:
			if elem is None and usedName == False:
				usedName = True
				ret.append(self.name)
			else:
				ret.append(elem)

		if usedName == False and self.name is not None:
			# Didn't fill in a placeholder, so stick it on the end.
			ret.append(self.name)
		return ret

def renameFunctionToMethod(funcname):
	name = funcname[4:]
	if name[1:2].lower() == name[1:2]:
		# then this is title case - lowercase the first letter
		return name[0:1].lower() + name[1:]
	else:
		return name

class IsStaticVisitor(c_ast.NodeVisitor):
	"""Call on a FuncDef."""
	def __init__(self, funcdef):
		self.isStatic = True
		self.visit(funcdef.decl.type.args)

	def visit_IdentifierType(self, node):
		if node.names == structtype:
			self.isStatic = False

class Method:
	def __init__(self, node):

		print "-------------------------"
		print "-- method --"
		statvisit = IsStaticVisitor(node)
		node.show(attrnames=True)
		self.static = statvisit.isStatic #isStatic(node.decl.type)
		self.retType = None
		self.params = None
		self.name = node.decl.name
		self.methodName = renameFunctionToMethod(self.name)
		self.location = node.decl.coord
		self.args = []
		for arg in node.decl.type.args.children():
			fullarg = TypeVisitor()
			fullarg.visit(arg)
			print "Argument:", fullarg.getFullType()
			self.args.append(fullarg)

		retType = TypeVisitor()
		retType.visit(node.decl.type.type)
		self.retType = retType.getTypeOnly() #getFullType(node.decl.type)


	def explain(self):
		print '%s: %s returns %s, takes:' % (context.location, context.name, context.retType)
		return

	def generateWrapper(self):
		# Static designation if needed
		if self.static:
			qualifiers = "static"
		else:
			qualifiers = ""

		# Return type
		### todo: handle pointer types correctly
		returntype = " ".join([str(x) for x in self.retType])

		# Method name
		declaration = self.methodName + "("

		# Method arguments
		typestring = lambda x: " ".join(x.getFullType())
		### todo: must also include the type of the arguments!
		if not self.static:
			# Drop the VC parameter
			self.args.pop(0)

		for x in self.args:
			print x.getFullType()

		# Forward the remaining parameters
		declaration += ( ", ".join([typestring(x) for x in self.args]) + ")")

		# Basic implementation - call original function
		body = "return "+ apicallqualifier + self.name + "("

		callargs = []
		if not self.static:
			callargs.append(virtcontextmember)

		callargs.extend([x.getNameOnly() for x in self.args])
		body += ", ".join(callargs)
		body += ");"

		return (qualifiers, returntype, declaration, body)

class FuncDefVisitor(c_ast.NodeVisitor):
	def __init__(self):
		self.wrapped_methods = []

	def visit_FuncDef(self, node):
		if node.decl.name in manuallywrapped:
			# manually wrapped, skip it.
			return

		## TODO - remove this debug output code
		#node.show(attrnames=True)

		# So, we need to wrap this method
		method = Method(node)
		self.wrapped_methods.append(method.generateWrapper())

def wrap_virtuose_api(filenames):
		# Note that cpp is used. Provide a path to your own cpp or
		# make sure one exists in PATH.
		#
		filename = None
		for fn in filenames:
			if os.path.exists(fn):
				filename = fn
				break
		if filename is None:
			print("Cannot continue: no valid input file found!")
			os.exit()

		ast = parse_file(filename, use_cpp=True, cpp_args=r'-Iutils/fake_libc_include')

		v = FuncDefVisitor()
		v.visit(ast)

		# Combined declaration and definition.
		#bodylines = [ " ".join([qualifiers, returntype, declaration, "{ " + body + " }"]) for (qualifiers, returntype, declaration, body) in v.wrapped_methods]
		#classbody = "\n\t\t".join(bodylines)
		#implbody = ""

		# separated declarations and definitions
		classlines = [ " ".join([returntype, declaration + ";"]) for (qualifiers, returntype, declaration, body) in v.wrapped_methods if qualifiers == ""]
		classlines.append("/* Static Methods */")
		classlines.extend([ " ".join([qualifiers, returntype, declaration + ";"]) for (qualifiers, returntype, declaration, body) in v.wrapped_methods if not qualifiers == ""])

		classbody = "\n\t\t".join(classlines)
		impllines = ["/* Wrapper Implementation Details Follow */"]
		impllines.extend([ " ".join(["inline", returntype, classname+"::"+declaration, "{\n\t" + body + "\n}"]) for (qualifiers, returntype, declaration, body) in v.wrapped_methods])
		implbody = "\n\n".join(impllines)

		boilerplatefile = open(bpfilename, 'r')
		boilerplate = string.replace(boilerplatefile.read(), includeplaceholder, os.path.basename(filename))
		boilerplatefile.close()


		classidx = boilerplate.find(classmarker)
		implidx = boilerplate.find(implmarker)

		if classidx != -1 and implidx != -1:
			fullfile = boilerplate[:classidx] + classbody + boilerplate[classidx+len(classmarker):implidx] + implbody + boilerplate[implidx+len(implmarker):]
		else:
			print "COULD NOT FIND PLACEHOLDER!"
			fullfile = None

		return fullfile


if __name__ == "__main__":
    if len(sys.argv) > 1:
        filename  = [sys.argv[1]]
    else:
        filename = defaultapifilenames

    output = wrap_virtuose_api(filename)
    #print output

    if len(sys.argv) > 2:
    	outfile = sys.argv[2]
    else:
    	outfile = defaultoutputfilename

  	out = open(outfile, 'w')
  	out.write(output)
  	out.close()
