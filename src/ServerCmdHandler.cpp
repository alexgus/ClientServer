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
		// TODO Check if the file exists
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
	vector<CmdOption*>::iterator it;

	if(this->cmd != NULL)
	{
		switch(this->cmd->getCmd())
		{
			case CmdLine::GET:
				it = cmd->getOptions()->begin();
				while(it != cmd->getOptions()->end() && (*it)->getOption() != "-f")
					++it;

				if(it == cmd->getOptions()->end())
				{ // TODO DEBUG
					cout << "No valid options" << endl;
					return -1;
				}
				else
				{
					SendFile *s = new SendFile((*it)->getValue(),port);
					s->send();
					return 1;
				}
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
