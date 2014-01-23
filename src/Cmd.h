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

class Cmd {
public:

	/**
	 * name cmd
	 */
	typedef enum{
		PUT = 1, GET = 2, QUIT = 3, ERR
	} CMD;

	/**
	 * Class' constructor
	 */

	Cmd(CMD cmd);

	Cmd(CMD cmd, list<string> &option, list<string> &arg);

	Cmd(string commande);

	/**
	 * Class' destructor
	 */
	virtual ~Cmd();

	/**
	 * getter
	 */

	CMD getCmd();

	list<string> getOption();

	list<string> getArg();

	/**
	 * Setter
	 */

	void setCmd(CMD cmd);

	void setOption(list<string> option);

	void setArg(list<string> arg);

	/**
	 * methode of add option or argument to the cmd
	 */

	void addOption(string option);

	void addArg(string arg);

	/*
	 * operator
	 */

	Cmd& operator=(Cmd cmd);

	friend std::ostream& operator<<(std::ostream& os, const Cmd& cmd);

private:

	CMD cmd;

	list<string> option;

	list<string> arg;
};

#endif /*CMD_H_ */

