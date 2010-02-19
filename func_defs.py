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


# A simple visitor for FuncDef nodes that prints the names and
# locations of function definitions.
#
class FuncDefVisitor(c_ast.NodeVisitor):
    def visit_FuncDef(self, node):
        func_decl = node.decl.type
        print '%s at %s: takes:' % (node.decl.name, node.decl.coord)
        for arg in func_decl.args.children():
        	print "   " + arg.name
#        print node.decl.func_decl.param_decls
        #node.show()
#        if node.decl.FuncDecl.ParamList.Decl.TypeDecl.IdentifierType == 'VirtContext':
 #           print


def show_func_defs(filename):
    # Note that cpp is used. Provide a path to your own cpp or
    # make sure one exists in PATH.
    #
    ast = parse_file(filename, use_cpp=True, cpp_args=r'-Iutils/fake_libc_include')

    v = FuncDefVisitor()
    v.visit(ast)


if __name__ == "__main__":
    if len(sys.argv) > 1:
        filename  = sys.argv[1]
    else:
        filename = 'VirtuoseAPI.h'

    show_func_defs(filename)
