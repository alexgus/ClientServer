/*
 * ServerCmdHandler.cpp
 *
 *  Created on: 25 janv. 2014
 *      Author: alexandre
 */

#include "ServerCmdHandler.h"

const string ServerCmdHandler::PYTHON_FILE = "server";
const string ServerCmdHandler::PYTHON_FUN = "recv";

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
	this->pyFile = g_conf.Get("Python","SERVER_FILE",this->PYTHON_FILE);
	this->pyFun = g_conf.Get("Python","SERVER_RECV",this->PYTHON_FUN);
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
				if(!(fstream(this->pyFile+".py").is_open()))
				{
					log.write("File " + this->pyFile + ".py not found !",typeid(*this).name(), Log::DBG);
					throw new string("File " + this->pyFile + ".py not found !");
				}
				this->pyCmd = new PythonModuleLoader(this->pyFile,this->pyFun);
				int ret = this->pyCmd->exec(*this->pyModuleStr);
				delete this->pyCmd;
				return ret;
				break;
		}
	}
	return -1;
}
