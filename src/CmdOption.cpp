/*
 * CmdOption.cpp
 *
 *  Created on: 5 juil. 2014
 *      Author: alexandre
 */

#include "CmdOption.h"

CmdOption::CmdOption()
{
	this->option = "";
	this->option = "";
}

CmdOption::~CmdOption()
{
}

bool CmdOption::valueExist()
{
	return (this->value != "");
}
