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
	this->pyCmd = NULL;
	this->pyModuleStr = NULL;
}

ServerCmdHandler::ServerCmdHandler(CmdLine &c)
{
	this->cmd = &c;
	this->pyCmd = NULL;
	this->pyModuleStr = NULL;
}

ServerCmdHandler::ServerCmdHandler(string &s)
{
	this->cmd = new CmdLine(s);
	if(this->cmd->getCmd() == CmdLine::ERR)
	{
		 this->pyCmd = new PythonModuleLoader(string("server"),string("recv"));
		 this->pyModuleStr = &s;
	}
}

ServerCmdHandler::~ServerCmdHandler()
{
	delete this->cmd;
	delete this->pyCmd;
}

ServerCmdHandler& ServerCmdHandler::operator=(ServerCmdHandler &s)
{
	return s;
}

int ServerCmdHandler::exec(int port)
{
	if(this->cmd != NULL)
	{
		list<string> arg = this->cmd->getArg();
		list<string> opt = this->cmd->getOption();
		list<string>::iterator it;

		//SendFile *send;

		switch(this->cmd->getCmd())
		{
			case CmdLine::GET:
				/*send = new SendFile(arg.front(),port);
				send->send();*/
				return 1;
				break;
			case CmdLine::PUT:
				return 1;
				break;
			case CmdLine::QUIT:
				return 0;
				break;
			case CmdLine::ERR:
				return this->pyCmd->exec(*this->pyModuleStr);
				break;
		}
	}
	return -1;
}
