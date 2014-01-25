/*
 * ServerCmdHandler.cpp
 *
 *  Created on: 25 janv. 2014
 *      Author: alexandre
 */

#include "ServerCmdHandler.h"

ServerCmdHandler::ServerCmdHandler()
{
	this->cmd = NULL;
}

ServerCmdHandler::ServerCmdHandler(Cmd &c)
{
	this->cmd = &c;
}

ServerCmdHandler::ServerCmdHandler(string &s)
{
	this->cmd = new Cmd(s);
}

ServerCmdHandler::~ServerCmdHandler()
{
	delete this->cmd;
}

ServerCmdHandler& ServerCmdHandler::operator=(ServerCmdHandler &s)
{
	return s;
}

void ServerCmdHandler::exec()
{
	if(this->cmd != NULL)
	{
		list<string> arg = this->cmd->getArg();
		list<string> opt = this->cmd->getOption();
		list<string>::iterator it;

		switch(this->cmd->getCmd())
		{
			case Cmd::GET:
				break;
			case Cmd::PUT:
				break;
			case Cmd::QUIT:
				break;
			case Cmd::ERR:
				break;
		}
	}
}
