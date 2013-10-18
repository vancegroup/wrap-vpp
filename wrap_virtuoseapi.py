#! /usr/bin/env python
# -*- coding: latin-1 -*-

# Initially based slightly on:
#-----------------------------------------------------------------
# pycparser: func_defs.py
#
# Using pycparser for printing out all the functions defined in a
# C file.
#
# This is a simple example of traversing the AST generated by
# pycparser.
#
# Copyright (C) 2008-2011, Eli Bendersky
# License: BSD
#-----------------------------------------------------------------

from __future__ import print_function
import sys
import os
import string
import re
from pycparser import c_parser, c_ast, parse_file, c_generator

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

methodTrans = { 'GetControlerVersion' : 'GetControllerVersion' }

bpfilename = "vpp-boilerplate.h"
classmarker = "/* CLASS BODY GOES HERE */"
implmarker = "/* IMPLEMENTATION BODY GOES HERE */"
includeplaceholder = "UPSTREAM_INCLUDE_FILENAME"
versionplaceholder = "UPSTREAM_VERSION_GOES_HERE"
intversionplaceholder = "UPSTREAM_INTEGER_VERSION_GOES_HERE"

virtcontextmember = "vc_"


defaultoutputfilename = 'vpp.h'
verbose = False

translateArg = lambda x: argTrans.get(x, x)
translateMethod = lambda x: methodTrans.get(x, x)

def getVersionStringFromHeader(fn):
	infile = open(fn, 'r')
	indata = infile.read()
	infile.close()
	m = re.search('Version number:[^0-9]*(?P<ver>[0-9]+\.[0-9]+)', indata)
	if m:
		return m.group('ver') # named match group
	else:
		return "Unknown"

def debugPrint(*args):
	if verbose:
		print(args)
	pass

class TypeVisitor(c_ast.NodeVisitor):
	def __init__(self):
		self.type = []
		self.name = None
		debugPrint("-------------------------")

	def visit_PtrDecl(self, node):
		# recurse first
		self.generic_visit(node)

		# add a pointer symbol only if this isn't a function pointer
		if not "(*" in self.type:
			self.type.append('*')
		debugPrint("after visit_PtrDecl:", self.type)

	def visit_IdentifierType(self, node):
		# recurse first
		self.generic_visit(node)

		# add typenames
		self.type.extend(node.names)
		debugPrint("after visit_IdentifierType:", self.type)

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
			debugPrint("Argument:", fullarg.getFullType())
			args.append(" ".join(fullarg.getFullType()))

		# Append it, wrapped in parens
		self.type.append("(" + ", ".join(args) + ")")

		debugPrint("after visit_FuncDecl:", self.type)

	def visit_TypeDecl(self, node):
		if node.declname is not None:
			self.name = translateArg(node.declname)
		self.generic_visit(node)
		debugPrint("after visit_TypeDecl:", self.type)

	def visit_Decl(self, node):
		if node.name is not None:
			self.name = translateArg(node.name)
		self.generic_visit(node)
		debugPrint("after visit_Decl:", self.type)

	def getTypeOnly(self):
		return " ".join(self.type)

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

class ChangeNameVisitor(c_ast.NodeVisitor):
	def __init__(self, nameFunctor):
		self.functor = nameFunctor

	def doTranslation(self, node):
		for attr in ["name", "declname"]:
			val = getattr(node, attr, None)
			if val is not None:
				newval = self.functor(val)
				if newval != val:
					setattr(node, attr, newval)

		if hasattr(node, "children"):
			self.generic_visit(node)

		# Must be sure to recurse Types
		if hasattr(node, "type"):
			self.doTranslation(node.type)


	def visit_Typename(self, node):
		self.doTranslation(node)

	def visit_Decl(self, node):
		self.doTranslation(node)

class TypeWrapper(object):
	def __init__(self, t):
		self.contained = t
		changer = ChangeNameVisitor(translateArg)
		changer.visit(self.contained)
		if verbose:
			self.contained.show(attrnames=True, nodenames=True)

	def getTypeOnly(self):
		generator = c_generator.CGenerator()
		return generator.visit(self.contained)

	def getNameOnly(self):
		if hasattr(self.contained, "name"):
			return self.contained.name if self.contained.name else ''
		elif hasattr(self.contained.type, "declname"):
			return self.contained.type.declname

	def changeName(self, oldName, name):
		functor = lambda x: name if x == oldName else x
		changer = ChangeNameVisitor(functor)
		changer.visit(self.contained)

	def getFullType(self):
		generator = c_generator.CGenerator()
		return generator.visit(self.contained)

def renameFunctionToMethod(funcname):
	name = translateMethod(funcname[4:])
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
		debugPrint("-------------------------")
		debugPrint("-- method --")
		statvisit = IsStaticVisitor(node)
		if verbose:
			node.show(attrnames=True, nodenames=True)
		self.static = statvisit.isStatic #isStatic(node.decl.type)
		self.retType = None
		self.params = None
		self.name = node.decl.name
		self.methodName = renameFunctionToMethod(self.name)
		self.location = node.decl.coord
		self.args = []
		for c_name, arg in node.decl.type.args.children():
			self.args.append(TypeWrapper(arg))

		if not self.static:
			# Drop the VC parameter
			self.args.pop(0)

		retType = TypeVisitor()
		retType.visit(node.decl.type.type)
		self.retType = retType.getTypeOnly() #getFullType(node.decl.type)

	def explain(self):
		print('%s: %s returns %s, takes:' % (context.location, context.name, context.retType))

	def callWrappedFunction(self):
		forwardCall = apicallqualifier + self.name + "("

		callargs = []
		if not self.static:
			callargs.append(virtcontextmember)

		callargs.extend([x.getNameOnly() for x in self.args])
		forwardCall += ", ".join(callargs)
		forwardCall += ")"
		return forwardCall

	def generateWrapper(self):
		# Static designation if needed
		if self.static:
			qualifiers = "static"
		else:
			qualifiers = ""

		# Return type
		### todo: handle pointer types correctly
		returntype = self.retType

		# Method name
		declaration = self.methodName + "("

		# Method arguments
		### todo: must also include the type of the arguments!

		for x in self.args:
			debugPrint(x.getFullType())

		# Forward the remaining parameters
		declaration += ( ", ".join([x.getFullType() for x in self.args]) + ")")

		# Basic implementation - call original function

		body = ""
		if returntype == "int" and not self.static:
			# Convert return codes into exceptions
			body = """
			int ret;
			VPP_CHECKED_CALL(ret = """ + self.callWrappedFunction() + """);
			return ret;"""
		else:
			# Doesn't return a return code.
			body = "return " + self.callWrappedFunction() + ";"

		return (qualifiers, returntype, declaration, body)

class AccumulateFuncDefsAsMethods(c_ast.NodeVisitor):
	def __init__(self):
		self.methods = []

	def visit_FuncDef(self, node):
		# So, we need to wrap this method - manually-wrapped methods are filtered
		# out later
		self.methods.append(Method(node))

class VirtuoseAPI:
	def __init__(self, fn):
		self.filename = fn
		self.apiVersion = getVersionStringFromHeader(self.filename)
		self.methods = None
		self.wrapped_methods = None

	def getApiVersion(self):
		return self.apiVersion

	def getApiVersionInteger(self):
		return reduce(
				lambda prev, newest: prev * 1000 + newest,
				[int(x) for x in self.getApiVersion().split(".")]
			)

	def parseFile(self):
		ast = parse_file(self.filename, use_cpp=True, cpp_args=r'-Iutils/fake_libc_include')

		v = AccumulateFuncDefsAsMethods()
		v.visit(ast)
		self.methods = v.methods

	def getMethods(self):
		if self.methods is None:
			self.parseFile()
		return self.methods

	def getWrappedMethods(self):
		if self.wrapped_methods is None:
			self.wrapped_methods = [ method.generateWrapper() for method in self.getMethods() if method.name not in manuallywrapped]
		return self.wrapped_methods

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

		API = VirtuoseAPI(filename)
		apiVer = API.getApiVersion()

		intVer = str(API.getApiVersionInteger())

		# Combined declaration and definition.
		#bodylines = [ " ".join([qualifiers, returntype, declaration, "{ " + body + " }"]) for (qualifiers, returntype, declaration, body) in v.wrapped_methods]
		#classbody = "\n\t\t".join(bodylines)
		#implbody = ""

		# separated declarations and definitions
		classlines = [ " ".join([returntype, declaration + ";"]) for (qualifiers, returntype, declaration, body) in API.getWrappedMethods() if qualifiers == ""]
		classlines.append("/* Static Methods */")
		classlines.extend([ " ".join([qualifiers, returntype, declaration + ";"]) for (qualifiers, returntype, declaration, body) in API.getWrappedMethods() if not qualifiers == ""])

		classbody = "\n\t\t".join(classlines)
		impllines = ["/* Wrapper Implementation Details Follow */"]
		impllines.extend([ " ".join(["inline", returntype, classname+"::"+declaration, "{\n\t" + body + "\n}"]) for (qualifiers, returntype, declaration, body) in API.getWrappedMethods()])
		implbody = "\n\n".join(impllines)

		boilerplatefile = open(bpfilename, 'r')
		boilerplate = boilerplatefile.read().replace(includeplaceholder, os.path.basename(filename)).replace(versionplaceholder, apiVer).replace(intversionplaceholder, intVer)
		boilerplatefile.close()


		classidx = boilerplate.find(classmarker)
		implidx = boilerplate.find(implmarker)

		if classidx != -1 and implidx != -1:
			fullfile = boilerplate[:classidx] + classbody + boilerplate[classidx+len(classmarker):implidx] + implbody + boilerplate[implidx+len(implmarker):]
		else:
			print("COULD NOT FIND PLACEHOLDER!")
			fullfile = None

		return fullfile, apiVer


if __name__ == "__main__":
	if len(sys.argv) > 1:
		filenames  = [sys.argv[1]]
		if len(sys.argv) > 2:
			outfile = sys.argv[2]
		else:
			outfile = "vpp-VER.h"
	else:
		filenames = defaultapifilenames
		outfile = defaultoutputfilename

	output, ver = wrap_virtuose_api(filenames)
	outfile = outfile.replace("VER", ver)

	print("Writing to file ", outfile)
	out = open(outfile, 'w')
	out.write(output)
	out.close()
