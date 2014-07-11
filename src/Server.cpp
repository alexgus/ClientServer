/*
 * Server.cpp
 *
 *  Created on: 11 nov. 2013
 *      Author: Alexandre Guyon
 */

#include "Server.h"

const int Server::PORT = 2371;

Server::Server()
{
	log.write("Initialize Server",typeid(*this).name(),Log::DBG);

	this->lClient = new vector<ClientData*>();

	// Create the socket
	fd_sock = socket(AF_INET, SOCK_STREAM, 0);
	if(fd_sock == -1)
		log.write("Socket creation error : " + string(strerror(errno)),typeid(*this).name(),Log::ERR);

	int optVal =1;
	if(setsockopt(fd_sock,SOL_SOCKET,SO_REUSEADDR ,&optVal,sizeof(int)) == -1)
		log.write("Server : Failed to change socket option : " + string(strerror(errno)),typeid(*this).name(),Log::ERR);

	// Bind the socket to the system
	addr.sin_family = AF_INET;
	addr.sin_addr.s_addr = htonl(INADDR_ANY); // Conversion big indian
	addr.sin_port = htons(PORT);
	if(bind(fd_sock,(struct sockaddr *) &addr, sizeof(addr)) == -1)
		log.write("Binding error : " + string(strerror(errno)),typeid(*this).name(),Log::ERR);

	// Set the listen queue
	listen(fd_sock, 5); // 5 = limit size of the connection queue
}

Server::Server(int port)
{
	log.write("Initialize Server",typeid(*this).name(),Log::DBG);

	this->lClient = new vector<ClientData*>();

	// Create the socket
	fd_sock = socket(AF_INET, SOCK_STREAM, 0);
	if(fd_sock == -1)
		log.write("Socket creation error : " + string(strerror(errno)),typeid(*this).name(),Log::ERR);

	int optVal =1;
	if(setsockopt(fd_sock,SOL_SOCKET,SO_REUSEADDR ,&optVal,sizeof(int)) == -1)
		log.write("Server : Failed to change socket option : " + string(strerror(errno)),typeid(*this).name(),Log::ERR);

	// Bind the socket to the system
	addr.sin_family = AF_INET;
	addr.sin_addr.s_addr = htonl(INADDR_ANY); // Conversion big indian
	addr.sin_port = htons(port);
	if(bind(fd_sock,(struct sockaddr *) &addr, sizeof(addr)) == -1)
		log.write("Binding error : " + string(strerror(errno)),typeid(*this).name(),Log::ERR);

	// Set the listen queue
	listen(fd_sock, 5); // 5 = limit size of the connection queue
}

Server::~Server()
{
	if(fd_sock != -1)
		close(fd_sock);
	delete this->lClient;
}


void Server::acceptConnection()
{
// Declaration
	int fd;
	int ret_select;
	fd_set read;
	timeval timeoutAccept;

	sockaddr *addrClient;
	socklen_t lenAddrClient = sizeof(sockaddr);;

	ClientData *data;

// Begin
	// If no socket
	if(fd_sock < 0)
		return;

	log.write("Server wait for connection",typeid(*this).name(),Log::DBG);

	this->mt_waitAccept.lock();
	while(this->waitAccept)
	{
		this->mt_waitAccept.unlock();

		// Set the fd_set
		ret_select = 0;
		timeoutAccept = { timeoutS , timeoutM };
		FD_ZERO(&read);
		FD_SET(fd_sock, &read);

		// Not wait on accept
		if((ret_select = select(fd_sock+1,&read,NULL,NULL,&timeoutAccept)) < 0)
		{
			log.write("Select accept error : " + string(strerror(errno)),typeid(*this).name(),Log::ERR);
			return;
		}

		// If there's an activity on the listen socket
		if(ret_select > 0)
		{
			addrClient = (sockaddr*) malloc(lenAddrClient);
			fd = accept(fd_sock, addrClient, &lenAddrClient);

			if(fd < 0)
				log.write("Accept error : " + string(strerror(errno)),typeid(*this).name(),Log::ERR);
			log.write("Server accepted connection",typeid(*this).name(),Log::DBG);

			// Get informations about client on fd
			Com *c = new Com(fd,{timeoutS,timeoutM});
			struct statvfs *fsShared = (struct statvfs*) c->readBlob(sizeof(struct statvfs));
			struct statvfs *fsPrivate = (struct statvfs*) c->readBlob(sizeof(struct statvfs));
			string *arch = c->readString();

			// Stocking client info
			data = new ClientData(addrClient, fd, fsShared, fsPrivate, arch);
			data->setThread(new thread(&Server::run,this,data));
			this->lClient->push_back(data);
		}
	}

	// Delete all running instance
	for(vector<ClientData*>::iterator it = this->lClient->begin(); it != this->lClient->end();++it)
	{
		(*it)->stopClient();
		(*it)->getThread()->join();
		delete *it;
	}
}

void Server::run(ClientData *d)
{
	// If fd isn't file descriptor
	if(d->getFd() < 0)
		return;

	Com *c = new Com(d->getFd(),{timeoutS,timeoutM});
	string *str;

	d->lockRun();
	while(d->isRunning())
	{
		d->unlockRun();
		str = c->readString();
		if(*str != "")
		{
			log.write("Server ["+ *d->getIp() + ":" + *d->getPort() + "][RECV] : " + *str, typeid(*this).name(), Log::DBG);
			thread t(&Server::handleString,this,d,c,str);
			t.detach(); // TODO Not detach thread
		}
	}
	c->writeString("Bye !");
	log.write("Server finished",typeid(*this).name(),Log::DBG);
}

void Server::handleString(ClientData *d, Com *c, string* s)
{
	ServerCmdHandler *cmdHandler = new ServerCmdHandler(*s);

	try
	{
		switch(cmdHandler->exec(d->getFd()))
		{
			case 0: // QUIT
				d->stopClient();
				break;
			case 1:
				c->writeString("OK : " + *s);
				break;
			case -1:
				c->writeString("Command not recognized : " + *s);
				break;
		}
	}
	catch (string* err)
	{
		c->writeString(">> " + *err);
		delete err;
	}

	delete cmdHandler;
}

void Server::stopServer()
{
	this->mt_waitAccept.lock();
	this->waitAccept = false;
	this->mt_waitAccept.unlock();
}
