#include "PythonModuleServer.h"

PythonModuleServer::PythonModuleServer()
{
	PyObject *pName, *pModule;

	Py_Initialize();
	PySys_SetPath(this->PATH_TO_MODULE); // Set the path to the module

	pName = PyUnicode_FromString(this->FILE.c_str()); // Convert the name of the module

	pModule = PyImport_Import(pName); // Load the module
	Py_DECREF(pName); // Free pName

	// If the module doesn't exist
	if (pModule != NULL)
	{
		pFunc = PyObject_GetAttrString(pModule, this->FUN.c_str()); // Load the function

		// If the function exist and is callable
		if (!(pFunc && PyCallable_Check(pFunc)))
		{
			if (PyErr_Occurred())
				PyErr_Print();
			fprintf(stderr, "Cannot find function \"%s\"\n", this->FUN.c_str());
		}
		Py_XDECREF(pFunc);
		Py_DECREF(pModule);
	}
	else
	{
		PyErr_Print();
		fprintf(stderr, "Failed to load \"%s\"\n", this->FILE.c_str());
	}
}

PythonModuleServer::~PythonModuleServer()
{
    Py_Finalize();
}

int PythonModuleServer::exec(string &s)
{
	PyObject *pValue, *pArgs;

	pArgs = PyTuple_New(1); // Set tuple for passing arguments
	PyTuple_SetItem(pArgs, 0, PyBytes_FromString(s.c_str())); // Set arguments

	pValue = PyObject_CallObject(pFunc, pArgs); // Call function
	Py_DECREF(pArgs); // pArgs useless now
	if (pValue != NULL)
	{
		int ret = (int) PyLong_AsLong(pValue);
		Py_DECREF(pValue);
		return ret;
	}
	else
	{
		PyErr_Print();
		fprintf(stderr,"Call failed\n");
		return 1;
	}
	return 0;
}
