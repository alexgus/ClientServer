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
	this->pyCmd = NULL;
	if(this->cmd->getCmd() == CmdLine::ERR)
		 this->pyModuleStr = &s;
}

ServerCmdHandler::~ServerCmdHandler()
{
	delete this->cmd;
}

ServerCmdHandler& ServerCmdHandler::operator=(ServerCmdHandler &s)
{
	return s;
}

int ServerCmdHandler::exec(int port) throw(string*)
{
	vector<CmdOption*>::iterator it;

	if(this->cmd != NULL)
	{
		switch(this->cmd->getCmd())
		{
			case CmdLine::GET:
				it = cmd->getOptions()->begin(); // Place on -f
				while(it != cmd->getOptions()->end() && (*it)->getOption() != "-f")
					++it;

				if(it == cmd->getOptions()->end())
					throw new string("No valid option !"); // TODO Add help
				if(!(fstream((*it)->getValue()).is_open()))
					throw new string("File " + (*it)->getValue() + " does not exist !");
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
				if(!(fstream(this->PYTHON_FILE+".py").is_open()))
				{
					log.write("File " + this->PYTHON_FILE + ".py not found !",typeid(*this).name(), Log::DBG);
					throw new string("File " + this->PYTHON_FILE + ".py not found !");
				}
				this->pyCmd = new PythonModuleLoader(this->PYTHON_FILE,this->PYTHON_FUN);
				int ret = this->pyCmd->exec(*this->pyModuleStr);
				delete this->pyCmd;
				return ret;
				break;
		}
	}
	return -1;
}
