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

from pycparser import c_parser, c_ast, parse_file

defaultapifilename = 'VirtuoseAPI.h'

structtype = ["VirtContext"]
apicallqualifier = "_VAPI::"
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
										

bpfilename = "vpp-boilerplate.hxx"
classmarker = "/* CLASS BODY GOES HERE */"
implmarker = "/* IMPLEMENTATION BODY GOES HERE */"


defaultoutputfilename = 'vpp.hxx'


def renameFunctionToMethod(funcname):
	name = funcname[4:]
	if name[1:2].lower() == name[1:2]:
		# then this is title case - lowercase the first letter
		return name[0:1].lower() + name[1:]
	else:
		return name

def isStatic(func_decl):
	### TODO fix this
	return False
	print func_decl.args.children()[0].type.name
	if func_decl.args.children()[0].type.name == structtype:
		return False
	else:
		return True

class IsStaticVisitor(c_ast.NodeVisitor):
	"""Call on a FuncDef."""
	def __init__(self, funcdef):
		self.isStatic = True
		self.visit(funcdef.decl.type.args)
	def visit_IdentifierType(self, node):
		if node.names == structtype:
			self.isStatic = False

class MethodWrapperVisitor(c_ast.NodeVisitor):
	def __init__(self, node):
		statvisit = IsStaticVisitor(node)
		self.static = statvisit.isStatic #isStatic(node.decl.type)
		self.retType = None
		self.params = None
		self.name = node.decl.name
		self.methodName = renameFunctionToMethod(self.name)
		self.location = node.decl.coord
		self.args = node.decl.type.args.children

	def visit_TypeDecl(self, node):
		if node.declname == self.name:
			# Then this is the declaration of the return type.
			# WARNING: right now it does not handle pointers properly -
			# since the only function returning a pointer as of 20100331 is
			# virtGetErrorMessage, we simply manually wrapped it.
			## TODO - handle pointer types properly here
			self.retType = node.type.names
		else:
			# This is a type for an argument!
			### TODO (?)
			pass

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
		
		translateArg = lambda x: argTrans.get(str(x.name),str(x.name))
		
		### todo: must also include the type of the arguments!
		if self.static:
			# No implicit VC parameter - forward them all
			declaration += ( ", ".join([translateArg(x)
				for x
				in self.args()]) +
				")")
		else:
			# Drop the VC parameter
			declaration += ( ", ".join([translateArg(x)
				for x
				in self.args()[1:]]) +
				")")
		
		# Basic implementation - call original function
		body = "return "+ apicallqualifier + self.name + "("
		if self.static:
			callargs = []
			forwardargs = self.args()[:]
		else:
			callargs = ["m_vc"]
			forwardargs = self.args()[1:]

		callargs.extend([translateArg(x) for x in forwardargs])
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
		
		node.show(attrnames=True)
		# So, we need to wrap this method
		method = MethodWrapperVisitor(node)
		method.visit(node)
		self.wrapped_methods.append(method.generateWrapper())

def wrap_virtuose_api(filename):
		# Note that cpp is used. Provide a path to your own cpp or
		# make sure one exists in PATH.
		#
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
		boilerplate = boilerplatefile.read()
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
        filename  = sys.argv[1]
    else:
        filename = defaultapifilename
    
    output = wrap_virtuose_api(filename)
    #print output
    
    if len(sys.argv) > 2:
    	outfile = sys.argv[2]
    else:
    	outfile = defaultoutputfilename
    	
  	out = open(outfile, 'w')
  	out.write(output)
  	out.close()

