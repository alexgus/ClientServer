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

string* convUByteToString(unsigned short ubyte, int nbByte = 1)
{
	char *buf = (char*) malloc(sizeof(char)*4);
	unsigned short nb = 0;

	for(int i = (7 * nbByte) + (nbByte - 1); i >= 0; --i)
	{
		//--- Get the ieme bit
		unsigned short w = (ubyte >> i) & 0b00000001;

		nb += w * pow(2,i);
	}

	sprintf(buf, "%hu", nb);

	return new string(buf);
}

string* ClientData::getIp()
{
	string *res;
	string *ip[4];

	ip[0] = convUByteToString(this->addrClient->sa_data[2]);
	ip[1] = convUByteToString(this->addrClient->sa_data[3]);
	ip[2] = convUByteToString(this->addrClient->sa_data[4]);
	ip[3] = convUByteToString(this->addrClient->sa_data[5]);

	res = new string(*ip[0] + string(".") + *ip[1] + string(".") + *ip[2] + string(".") + *ip[3]);

	return res;
}

string* ClientData::getPort()
{
	unsigned short port = (this->addrClient->sa_data[0] << 8) | this->addrClient->sa_data[1];
	string *res = new string(*convUByteToString(port,2));
	return res;
}
