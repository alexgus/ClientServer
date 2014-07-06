/*
 * ClientData.cpp
 *
 *  Created on: 6 juil. 2014
 *      Author: alexandre
 */

#include "ClientData.h"

ClientData::ClientData(sockaddr* s, int fd)
{
	this->t = 0;
	this->addrClient = s;
	this->fd = fd;
	this->run = true;
	this->mRun = new mutex();
}

ClientData::~ClientData()
{
	delete this->mRun;
	delete this->t;
	free(this->addrClient);
	close(fd);
}

void ClientData::stopClient()
{
	this->mRun->lock();
	this->run = false;
	this->mRun->unlock();

	this->t->join();
}

string& ClientData::getIp()
{
	string *s = new string("0.0.0.0");
	return *s;
}

string& ClientData::getPort()
{
	string *s = new string("0");
	return *s;
}
