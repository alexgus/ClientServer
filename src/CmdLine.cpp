/*
 * Cmd.c
 *
 *  Created on: 20 janv. 2014
 *      Author: kevin
 */

#include "CmdLine.h"

CmdLine::CmdLine()
{
	this->cmd = CMD::ERR;
	this->options = new vector<CmdOption*>();
}

CmdLine::CmdLine(CMD cmd)
{
	this->cmd = cmd;
	this->options = new vector<CmdOption*>();
}

CmdLine::CmdLine(CMD cmd, vector<CmdOption*> *o)
{
	this->cmd = cmd;
	this->options = o;
}

CmdLine::CmdLine(string commande)
{
	this->cmd = CMD::GET;
	this->options = new vector<CmdOption*>();

	string str;
	auto it = commande.cbegin();
	//
	while ((it != commande.cend()) && (*it != ' '
			&& *it != '\n'
			&& *it != '\r'
			&& *it != '\0'))
	{
		str = str + *it;
		it++;
	}

	if(str == "GET")
		this->cmd = GET;
	else if (str == "PUT")
		this->cmd = PUT;
	else if (str == "QUIT")
		this->cmd = QUIT;
	else
		this->cmd = ERR;

	str = "";

	//search option
	while (it != commande.cend())
	{
		if(*it!=' ')
		{
			// Search end of the option name
			str = "";
			while ((it != commande.cend())&& ((*it != ' ')))
											/*|| (*it != '\r')
											|| (*it != '\n')))*/
			{
				str = str + *it;
				++it;
			}

			// Insert name of the option
			CmdOption *o = new CmdOption();
			o->setOption(str);

			// Search for value
			if(it != commande.cend())
			{
				// Search argument
				while ((it != commande.cend())&& (*it == ' '))
					++it;

				if(it != commande.cend() && *it != '-')
				{
					str = "";

					while ((it != commande.cend())&& ((*it != ' ')))
													/*|| (*it != '\r')
													|| (*it != '\n')))*/
					{
						str = str + *it;
						++it;
					}

					// Set the value and add it to the vector
					o->setValue(str);
				}
			}

			this->options->push_back(o);
		}
		else
			it++;
	}
}

/**
 * Class' destructor
 */
CmdLine::~CmdLine()
{

}

CmdLine& CmdLine::operator=(const CmdLine others)
{
	this->cmd = others.cmd;
	this->options = others.options;
	return *this;
}

std::ostream& operator<<(ostream& os, const CmdLine& cmd)
{
	switch (cmd.cmd)
	{
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

	os << endl;

	return os;
}
