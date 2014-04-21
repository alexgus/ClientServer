def help():
	print("Help in using the python module :")
	print("TO COMPLETE ...")

def testPython():
	print("Python module is running normally")
	help()

def recv(string):
	# Convert c-PyString to Python string
	string = string.decode("utf-8")
	
	if string == 'TESTPY':
		testPython()
		return 1
	else:
		return -1