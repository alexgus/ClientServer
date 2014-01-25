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
#include "Log.h"
#include "Cmd.h"

using namespace std;

/**
 * Handle commands sent by the client to the server
 */
class ServerCmdHandler
{
public:
	/**
	 * Class constructor.
	 */
	ServerCmdHandler();

	/**
	 * Class constructor.
	 * @param c Command to handle
	 */
	ServerCmdHandler(Cmd &c);

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
	 */
	void exec();

	/**
	 * Override operator =
	 */
	ServerCmdHandler& operator=(ServerCmdHandler &s);

private:

	/**
	 * The command to handle
	 */
	Cmd* cmd;

	/**
	 * Log file
	 */
	Log log;
};

#endif /* SERVERCMDHANDLER_H_ */
