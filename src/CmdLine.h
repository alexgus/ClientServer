/*
 * Cmd.h
 *
 *  Created on: 20 janv. 2014
 *      Author: kevin
 */

#ifndef CMD_H_
#define CMD_H_

#include <string>
#include <iostream>
#include <list>
#include <iterator>
using namespace std;

class CmdLine
{
public:
	/**
	 * Define all Commands
	 */
	typedef enum
	{
		PUT = 1,
		GET = 2,
		QUIT = 3,
		ERR
	} CMD;

	/**
	 * Class' constructors
	 */
	CmdLine();
	CmdLine(CMD cmd);
	CmdLine(CMD cmd, list<string> &option, list<string> &arg);
	CmdLine(string commande);

	/**
	 * Class' destroyer
	 */
	virtual ~CmdLine();

	/**
	 * Get the command in the line
	 */
	CMD getCmd();

	/**
	 * Get options of the command
	 * @return (list) Options of the command
	 */
	list<string> getOption();

	/**
	 * Get arguments of the command
	 * @return (list) Arguments of the command
	 */
	list<string> getArg();

	/**
	 * Set the command in the line
	 */
	void setCmd(CMD cmd);

	/**
	 * Set options in the commands
	 * @param option
	 */
	void setOption(list<string> option);

	/**
	 * Set Arguments of the arguments
	 * @param arg
	 */
	void setArg(list<string> arg);

	/**
	 * Method for adding options to the command line
	 */
	void addOption(string option);

	/**
	 * Method for adding arguments to the command line
	 * @param arg
	 */
	void addArg(string arg);

	/**
	 * Assign a command line to the current object
	 * @param cmd The command line to assign
	 * @return The new command line
	 */
	CmdLine& operator=(CmdLine cmd);

	/**
	 * Print method of the command line
	 * @param os output stream to write
	 * @param cmd The command line to write
	 * @return the output stream written
	 */
	friend std::ostream& operator<<(std::ostream& os, const CmdLine& cmd);

private:

	/**
	 * The command
	 */
	CMD cmd;

	/**
	 * List of options
	 */
	list<string> option;

	/**
	 * List of arguments
	 */
	list<string> arg;
};

#endif /*CMD_H_ */

