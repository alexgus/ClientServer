/*
 * Cmd.c
 *
 *  Created on: 20 janv. 2014
 *      Author: kevin
 */

#include "Cmd.h"

CmdLine::CmdLine()
{
	this->cmd = CMD::ERR;
}

CmdLine::CmdLine(CMD cmd) {
	this->cmd = cmd;
}

CmdLine::CmdLine(CMD cmd, list<string> &option, list<string> &arg) {
	this->cmd = cmd;
	this->option = option;
	this->arg = arg;
}

// FIXME Delete \r\n at the end of the string
CmdLine::CmdLine(string commande) {
	this->cmd = CMD::GET;

	string str;
	auto it = commande.cbegin();
	//
	while ((*it != ' ') && (it != commande.cend())) {
		str = str + *it;
		it++;
	}
	if(str == "GET")
		this->setCmd(CmdLine::GET);
	else if (str == "PUT")
		this->setCmd(CmdLine::PUT);
	else if (str == "QUIT")
		this->setCmd(CmdLine::QUIT);
	else
		this->setCmd(CmdLine::ERR);
	str = "";

	//search option and arg
	while (it != commande.cend()){
		if(*it!=' ')
		{
			str = "";
			while ((*it != ' ') && (it != commande.cend())){
				str = str + *it;
				it++;
			}
			//add option whitout '-'
			if(str[0] == '-')
				this->addOption(str.assign(str.cbegin()+1, str.cend()));
			//add arg
			else
				this->addArg(str);
		}
		else
			it++;
	}
}

/**
 * Class' destructor
 */
CmdLine::~CmdLine() {

}

/**
 * getter
 */

CmdLine::CMD CmdLine::getCmd() {
	return this->cmd;
}

list<string> CmdLine::getOption() {
	return this->option;
}

list<string> CmdLine::getArg() {
	return this->arg;
}

/**
 * Setter
 */

void CmdLine::setCmd(CMD cmd) {
	this->cmd = cmd;
}

void CmdLine::setOption(list<string> option) {
	this->option = option;
}

void CmdLine::setArg(list<string> arg) {
	this->arg = arg;
}

/**
 * methode of add option or argument to the cmd
 */

void CmdLine::addOption(string option) {
	this->option.push_back(option);
}

void CmdLine::addArg(string arg) {
	this->arg.push_back(arg);
}

/*
 * operator
 */

CmdLine& CmdLine::operator=(const CmdLine others) {
	this->cmd = others.cmd;
	this->arg = others.arg;
	this->option = others.option;
	return *this;
}

std::ostream& operator<<(ostream& os, const CmdLine& cmd) {

	//list<string>::iterator it;

	switch (cmd.cmd) {
	case CmdLine::GET:
		os << "GET";
		break;
	case CmdLine::PUT:
		os << "PUT";
		break;
	case CmdLine::QUIT:
		os << "QUIT";
		break;
	default:
		os << "ERR";
		break;
	}

	list<string>::iterator it;
	list < string > list = cmd.option;

	if (!cmd.option.empty()) {
		for (it = list.begin(); it != list.end(); ++it)
			os << " -" << *it;
	}

	list = cmd.arg;
	if (!cmd.arg.empty()) {
		for (it = list.begin(); it != list.end(); it++)
			os << ' ' << *it;
	}
	os << endl;

	return os;
}
