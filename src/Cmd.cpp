/*
 * Cmd.c
 *
 *  Created on: 20 janv. 2014
 *      Author: kevin
 */

#include "Cmd.h"

Cmd::Cmd()
{
	this->cmd = CMD::ERR;
}

Cmd::Cmd(CMD cmd) {
	this->cmd = cmd;
}

Cmd::Cmd(CMD cmd, list<string> &option, list<string> &arg) {
	this->cmd = cmd;
	this->option = option;
	this->arg = arg;
}

Cmd::Cmd(string commande) {
	this->cmd = CMD::GET;

	string str;
	auto it = commande.cbegin();
	//
	while ((*it != ' ') && (it != commande.cend())) {
		str = str + *it;
		it++;
	}
	if(str == "GET")
		this->setCmd(Cmd::GET);
	else if (str == "PUT")
		this->setCmd(Cmd::PUT);
	else if (str == "QUIT")
		this->setCmd(Cmd::QUIT);
	else
		this->setCmd(Cmd::ERR);
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
Cmd::~Cmd() {

}

/**
 * getter
 */

Cmd::CMD Cmd::getCmd() {
	return this->cmd;
}

list<string> Cmd::getOption() {
	return this->option;
}

list<string> Cmd::getArg() {
	return this->arg;
}

/**
 * Setter
 */

void Cmd::setCmd(CMD cmd) {
	this->cmd = cmd;
}

void Cmd::setOption(list<string> option) {
	this->option = option;
}

void Cmd::setArg(list<string> arg) {
	this->arg = arg;
}

/**
 * methode of add option or argument to the cmd
 */

void Cmd::addOption(string option) {
	this->option.push_back(option);
}

void Cmd::addArg(string arg) {
	this->arg.push_back(arg);
}

/*
 * operator
 */

Cmd& Cmd::operator=(const Cmd others) {
	this->cmd = others.cmd;
	this->arg = others.arg;
	this->option = others.option;
	return *this;
}

std::ostream& operator<<(ostream& os, const Cmd& cmd) {

	//list<string>::iterator it;

	switch (cmd.cmd) {
	case Cmd::GET:
		os << "GET";
		break;
	case Cmd::PUT:
		os << "PUT";
		break;
	case Cmd::QUIT:
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
