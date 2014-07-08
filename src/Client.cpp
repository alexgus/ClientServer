/*
 * Client.cpp
 *
 *  Created on: 20 Jan 2014
 *      Author: nicolas
 */

#include "Client.h"

Client::Client()
{
	this->running = false;
	this->cUser = new Com(fileno(stdin),{2,0});
	this->cSocket = 0;
	fd_sock = -1;

	host_info_list = 0;
	host_info.ai_family = AF_UNSPEC;     // IP version not specified. Can be both.
	host_info.ai_socktype = SOCK_STREAM; // Use SOCK_STREAM for TCP or SOCK_DGRAM for UDP.
}

Client::Client(string address, int port)
{
	this->running = false;
	fd_sock = -1;
	this->cUser = new Com(fileno(stdin),{2,0});
	this->cSocket = 0;

	host_info.ai_family = AF_UNSPEC;     // IP version not specified. Can be both.
	host_info.ai_socktype = SOCK_STREAM; // Use SOCK_STREAM for TCP or SOCK_DGRAM for UDP.
	initHostInfo(address, port);
}

Client::~Client()
{
	delete this->cSocket;
	delete this->cUser;
}


int Client::initHostInfo(string address, int port)
{
	char const * p = to_string(port).c_str();
	int status = getaddrinfo(address.c_str() , p, &host_info, &host_info_list);
	if(status != 0)
	{
		log.write("Client : Failed to get Hosts Info",typeid(*this).name(),Log::WARN);
		return status;
	}

	log.write("Client is ready to connect to the server",typeid(*this).name(),Log::DBG);

	return status;
}

int Client::connection()
{
	fd_sock = socket(host_info_list->ai_family,
			host_info_list->ai_socktype,
			host_info_list->ai_protocol);

	if (fd_sock == -1)
	{
		log.write("Client : Failed creating the socket",typeid(*this).name(),Log::ERR);
		return -1;
	}

	int status = connect(fd_sock, host_info_list->ai_addr, host_info_list->ai_addrlen);

	if (status == -1)
	{
		log.write("Client : Failed to connect to Host",typeid(*this).name(),Log::ERR);
		return -1;
	}

	this->cSocket = new Com(this->fd_sock,{2,0});

	// TODO server ask for information

	// TODO change dir
	struct statvfs *fs = (struct statvfs*) malloc(sizeof(struct statvfs));
	statvfs(".",fs);
	send(fd_sock,fs,sizeof(struct statvfs),0);

	// arm or x86 ?
	// MDP

	free(fs);

	return status;
}
void Client::run()
{
	string *msg=0, *rcv=0;

	// Connect to the server
	connection();

	this->running = true;

	while(this->running)
	{
		cout << "Client : type a command to send"<<endl;

		msg = this->cUser->readString();
		while(*msg == "")
			msg = this->cUser->readString();

		if(msg->length() >= 3)
		{
			this->cSocket->writeString(*msg);

			cout<<"Client : waiting for an answer .."<<endl;
			rcv = this->cSocket->readString();
			cout << "Client [RCV] : "<< *rcv <<endl;

			if(*rcv == "Bye !")
			{
				cout<<"Client exiting -- Closing socket"<<endl;
				close(fd_sock);
				return;
			}
		}
	}
}
