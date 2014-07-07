/*
 * ClientData.cpp
 *
 *  Created on: 6 juil. 2014
 *      Author: alexandre
 */

#include "ClientData.h"

string* convUByteToString(uint16_t ubyte)
{
	// TODO Verify indianness
	char *buf = (char*) malloc(sizeof(char)*4);

	sprintf(buf, "%hu", ubyte);

	return new string(buf);
}


ClientData::ClientData(sockaddr* s, int fd)
{
	this->t = 0;
	this->addrClient = s;
	this->fd = fd;
	this->run = true;
	this->mRun = new mutex();

	// Stock ip
	string *ip[4];
	ip[0] = convUByteToString(this->addrClient->sa_data[2]);
	ip[1] = convUByteToString(this->addrClient->sa_data[3]);
	ip[2] = convUByteToString(this->addrClient->sa_data[4]);
	ip[3] = convUByteToString(this->addrClient->sa_data[5]);
	this->ip = new string(*ip[0] + string(".") + *ip[1] + string(".") + *ip[2] + string(".") + *ip[3]);

	// Stock port
	unsigned short port = (this->addrClient->sa_data[0] << 8) | this->addrClient->sa_data[1];
	this->port = new string(*convUByteToString(port));
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
}
