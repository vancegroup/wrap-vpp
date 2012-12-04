#! /usr/bin/env python

import sys

from wrap_virtuoseapi import VirtuoseAPI

if __name__ == "__main__":
	if len(sys.argv) > 1:
		fn = sys.argv[1]
	else:
		fn = "virtuoseAPI.h"

	API = VirtuoseAPI(fn)
	for method in API.getMethods():
		print(method.name)
	
