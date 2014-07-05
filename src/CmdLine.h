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
#include <vector>
#include <iterator>

#include "CmdOption.h"
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
	CmdLine(CMD cmd, vector<CmdOption*> *o);
	CmdLine(string commande);

	/**
	 * Class' destroyer
	 */
	virtual ~CmdLine();

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

	/**
	 * Get options list
	 * @return Options list
	 */
	vector<CmdOption*>* getOptions() const
	{
		return options;
	}

	/**
	 * Set options list
	 * @param options options list
	 */
	void setOptions(vector<CmdOption*>* options)
	{
		this->options = options;
	}

	/**
	 * Get the current command
	 * @return The command
	 */
	CMD getCmd()
	{
		return cmd;
	}

	/**
	 * Set the current command
	 * @param c The command to set
	 */
	void setCmd(CMD c)
	{
		this->cmd = c;
	}

private:

	/**
	 * The command
	 */
	CMD cmd;

	/**
	 * List of options
	 */
	vector<CmdOption*> *options;
};

#endif /*CMD_H_ */

