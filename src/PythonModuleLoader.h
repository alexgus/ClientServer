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

class PythonModuleLoader
{
private:
	/**
	 * Path to find the module
	 */
	const wchar_t *PATH_TO_MODULE = L".";

	/**
	 * The python function callback
	 */
    PyObject *pFunc;

public:
	PythonModuleLoader(const string &file, const string &fun);
	virtual ~PythonModuleLoader();

	int exec(string &s);
};
