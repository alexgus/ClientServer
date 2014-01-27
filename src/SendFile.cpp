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
	char num[6];
	sprintf(num,"%d",clientPort);
	log.write("Sending file " + path + " to " + num,Log::DBG,"SendFile");

	this->com = new Com(clientPort,{2,0});
	this->file.open(path,ifstream::in);
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

	buf = (char*)malloc(len);

	while(!this->file.eof())
	{
		this->file.read(buf,len);
		log << buf;
		this->com->writeString(buf);
	}
}
