/*
 * Com.cpp
 *
 *  Created on: 25 janv. 2014
 *      Author: alexandre
 */

#include "Com.h"

Com::Com()
{
	this->fd = -1;
	this->timeout = {0,0};
}

Com::Com(int fd, timeval timeOut)
{
	this->fd = fd;
	this->timeout = timeOut;
}

Com::~Com()
{

}

void Com::writeString(const string &s)
{
	write(fd,s.c_str(),(unsigned int)s.size());
}

string* Com::readString()
{
	timeval t = this->timeout;
	int ret_select;
	fd_set readSet;

	char buf[BUFFER_SIZE];
	int nbRead = 0;
	string *str = new string("");

	ret_select = 0;
	FD_ZERO(&readSet);
	FD_SET(fd, &readSet);

	if((ret_select = select(fd+1,&readSet,NULL,NULL,&t)) < 0)
	{
		log.write("Select read error : " + string(strerror(errno)),typeid(*this).name(),Log::ERR);
		return str;
	}

	// the state of fd changed
	if(ret_select > 0)
	{
		// Read command
		nbRead = read(fd,buf,BUFFER_SIZE-1);
		buf[nbRead] = '\0'; // Add end of string

		delete str;
		str = new string(buf);
		return str;
	}

	return str;
}

void Com::writeBlob(void* buf, int size)
{
	write(this->fd,buf,size);
}

char* Com::readBlob(int size)
{
	char *buf = (char*) malloc(sizeof(char)*size);
	timeval t = this->timeout;
	int ret_select;
	fd_set readSet;

	ret_select = 0;
	FD_ZERO(&readSet);
	FD_SET(fd, &readSet);

	if((ret_select = select(fd+1,&readSet,NULL,NULL,&t)) < 0)
	{
		log.write("Select read error : " + string(strerror(errno)),typeid(*this).name(),Log::ERR);
		return 0;
	}

	// the state of fd changed
	if(ret_select > 0)
	{
		read(fd, buf, size);
		return buf;
	}
	return 0;
}
