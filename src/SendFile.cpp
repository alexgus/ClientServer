/*
 * SendFile.cpp
 *
 *  Created on: 27 janv. 2014
 *      Author: alexandre
 */

#include "SendFile.h"

SendFile::SendFile()
{
	this->com = NULL;
}

SendFile::SendFile(string path, int clientPort)
{
	log.write("Sending file",Log::DBG,"SendFile");

	this->com = new Com(clientPort,{2,0});
	this->file.open(path,ifstream::in);

	this->send();
}

SendFile::~SendFile()
{
	delete this->com;
}

void SendFile::send()
{
	int len;
	char *buf = NULL;

	this->file.seekg(0,this->file.end);
	len = this->file.tellg();
	this->file.seekg(0,this->file.beg);

	buf = new char[len];

	while(!this->file.eof())
	{
		this->file.read(buf,len);
		this->com->writeString(buf);
	}
}
