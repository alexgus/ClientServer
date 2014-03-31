/*
 * SendFile.cpp
 *
 *  Created on: 27 janv. 2014
 *      Author: alexandre
 */

#include "SendFile.h"

SendFile::SendFile()
{

}

SendFile::SendFile(string path, int clientPort)
{
	// Convert int to char
	char num[6];
	sprintf(num,"%d",clientPort);

	log.write("Sending file " + path + " to " + num,Log::DBG,typeid(this).name());

	this->com = new Com(clientPort,{2,0});
	this->file.open(path,ifstream::in);
}

SendFile::~SendFile()
{
	if(this->com != NULL)
		delete this->com;
}

void SendFile::send()
{
	int lenFile;
	int sendedBytes = 0;
	char *buf = NULL;

	// Find the len of the file
	this->file.seekg(0,this->file.end);
	lenFile = this->file.tellg();
	this->file.seekg(0,this->file.beg);

	log.write("The size of the file to send is " + lenFile,Log::DBG,typeid(this).name());

	// Allocates the buffer for reading
	if(lenFile < MAX_MTU)
		buf = (char*)malloc(lenFile);
	else
		buf = (char*)malloc(MAX_MTU);

	while(!this->file.eof())
	{
		// If the file doesn't need more than one buffer
		if(lenFile < MAX_MTU)
		{
			this->file.read(buf,lenFile);
			break;
		} // The file needed more than one buffer, but the curent data doeasn't need it
		else if((sendedBytes + MAX_MTU) >= lenFile)
		{
			this->file.read(buf,(lenFile - sendedBytes));
			break;
		}
		else // The file still need more than on buffer for complete reading
		{
			this->file.read(buf,MAX_MTU);
			sendedBytes += MAX_MTU;
		}

		// Write the buffer on socket
		this->com->writeString(buf);
	}
}
