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
	string *str;
	char *buf = (char*) malloc(sizeof(char)*4);

	sprintf(buf, "%hu", ubyte);

	str = new string(buf);
	free(buf);

	return str;
}


ClientData::ClientData(sockaddr* s, int fd, struct statvfs *fs, struct statvfs *fsP, string *arch)
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

	for(int i = 0; i< 4 ; ++i)
		delete ip[i];

	// Stock port
	this->port = convUByteToString((this->addrClient->sa_data[0] << 8) | this->addrClient->sa_data[1]);

	// Filesystem statistic
	this->statfsShared = fs;
	this->statfsPrivate = fsP;
	this->disk_sizeShared = fs->f_frsize * fs->f_blocks;
	this->disk_freeShared = fs->f_frsize * fs->f_bfree;
	this->disk_sizePrivate = fsP->f_frsize * fsP->f_blocks;
	this->disk_freePrivate = fsP->f_frsize * fsP->f_bfree;

	// Arch
	this->arch = arch;
}

ClientData::~ClientData()
{
	delete this->ip;
	delete this->port;
	delete this->mRun;
	delete this->t;
	delete this->arch;
	free(this->addrClient);
	free(this->statfsShared);
	close(fd);
}

void ClientData::stopClient()
{
	this->mRun->lock();
	this->run = false;
	this->mRun->unlock();
}
