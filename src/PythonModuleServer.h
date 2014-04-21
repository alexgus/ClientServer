/*
 * PythonModuleServer.h
 *
 *  Created on: 20 avr. 2014
 *      Author: alexandre
 */

#include <string>
#include <python3.4m/Python.h>

using namespace std;

// TODO Add log

class PythonModuleServer
{
private:
	/**
	 * Path to find the module
	 */
	const wchar_t *PATH_TO_MODULE = L".";

	/**
	 * Name of the module in the path (without extension)
	 */
	string FILE = "server";

	/**
	 * Name of the callback function in the module
	 */
	string FUN = "recv";

	/**
	 * The python function callback
	 */
    PyObject *pFunc;

public:
	PythonModuleServer();
	virtual ~PythonModuleServer();

	int exec(string &s);
};
