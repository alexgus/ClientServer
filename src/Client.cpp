/*
 * Client.cpp
 *
 *  Created on: 20 Jan 2014
 *      Author: nicolas
 */

#include "Client.h"

const int Client::PORT = 2371;
const string Client::HOST = "127.0.0.1";

Client::Client()
{
	this->running = false;
	this->mRunning = new mutex();
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
	this->mRunning = new mutex();
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
	char const *p = to_string(port).c_str();
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
	this->cSocket->writeBlob(fs,sizeof(struct statvfs));

	// arm or x86 ?
	string *arch = this->getArch();
	this->cSocket->writeString(*arch);
	// MDP

	free(fs);
	delete arch;

	return status;
}

string* Client::getArch()
{
	int fd[2];
	int status;

	status = pipe(fd);
	if(status == -1)
	{
		log.write("Problem when creating pipe for getting Architecture", typeid(*this).name(),Log::DBG);
		return new string("");
	}

	switch(fork())
	{
		case -1:
			log.write("Problem fork for getting Architecture", typeid(*this).name(),Log::DBG);
			return new string("");
			break;
		case 0: // Child
			close(fd[0]);
			dup2(fd[1],fileno(stdout));
			execlp("uname","uname","-m",0);
			break;
		default: // Parent
			close(fd[1]);
			Com *c = new Com(fd[0],{10,0});
			string *s =  c->readString();
			s->erase(s->end()-1); // Delete \n
			delete c;
			close(fd[0]);
			return s;
			break;
	}
	return new string("");
}

void Client::run()
{
	thread *send, *recv;

	// Connect to the server
	connection();

	this->running = true;

	recv = new thread(&Client::listenOn,this);
	send = new thread(&Client::writeCommand,this);

	recv->join();
	send->join();

	delete recv;
	delete send;
}

void Client::listenOn()
{
	string *rcv;

	while(true)
	{
		rcv = this->cSocket->readString();
		while(*rcv == "")
		{
			delete rcv;
			rcv = this->cSocket->readString();
		}
		cout << "Client [RCV] : "<< *rcv <<endl;

		if(*rcv == "Bye !")
		{
			cout<<"Client exiting"<<endl;
			close(fd_sock);
			this->mRunning->lock();
			this->running = false;
			this->mRunning->unlock();
			delete rcv;
			return;
		}
		delete rcv;
	}
}

void Client::writeCommand()
{
	string *msg = new string("");
	this->mRunning->lock();
	while(this->running)
	{
		this->mRunning->unlock();
		cout << "Client : type a command to send"<<endl;

		msg = this->cUser->readString();
		this->mRunning->lock();
		while(*msg == "" && this->running)
		{
			this->mRunning->unlock();
			delete msg;
			msg = this->cUser->readString();
		}

		this->cSocket->writeString(*msg);
		delete msg;
	}
}
