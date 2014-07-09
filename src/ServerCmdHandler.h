/*
 * ServerCmdHandler.h
 *
 *  Created on: 25 janv. 2014
 *      Author: alexandre
 */

#ifndef SERVERCMDHANDLER_H_
#define SERVERCMDHANDLER_H_

#include <iostream>
#include <string>
#include <vector>

#include "Log.h"
#include "CmdLine.h"
#include "CmdOption.h"
#include "SendFile.h"
#include "PythonModuleLoader.h"

using namespace std;

/**
 * Handle commands sent by the client to the server
 */
class ServerCmdHandler
{
public:

	/**
	 * File to the python module constant
	 */
	const  string PYTHON_FILE = "server";

	/**
	 * File to the python module constant
	 */
	const string PYTHON_FUN = "recv";

	/**
	 * Class constructor.
	 */
	ServerCmdHandler();

	/**
	 * Class constructor.
	 * @param c Command to handle
	 */
	ServerCmdHandler(CmdLine &c);

	/**
	 * Class constructor.
	 * @param s Command string to handle
	 */
	ServerCmdHandler(string &s);

	/**
	 * Destroyer of the class
	 */
	virtual ~ServerCmdHandler();

	/**
	 * Execute the command
	 * @return 1 if ok, -1 if not ok, 0 if quit command
	 * @throw Error string
	 */
	int exec(int port) throw(string*);

	/**
	 * Override operator =
	 */
	ServerCmdHandler& operator=(ServerCmdHandler &s);

private:

	/**
	 * The command to handle
	 */
	CmdLine* cmd;

	/**
	 * Log file
	 */
	Log log;

	/**
	 * For sending commands to python modules
	 */
	PythonModuleLoader *pyCmd;

	/**
	 * String for python module
	 */
	string *pyModuleStr;

};

#endif /* SERVERCMDHANDLER_H_ */
