/*
 * Server.cpp
 *
 *  Created on: 11 nov. 2013
 *      Author: Alexandre Guyon
 */

#include "Server.h"

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

		if((ret_select = select(fd_sock+1,&read,NULL,NULL,&timeoutAccept)) < 0)
		{
			log.write("Select accept error : " + string(strerror(errno)),typeid(*this).name(),Log::ERR);
			return;
		}

		if(ret_select > 0)
		{
			addrClient = (sockaddr*) malloc(lenAddrClient);
			fd = accept(fd_sock, addrClient, &lenAddrClient);

			if(fd < 0)
				log.write("Accept error : " + string(strerror(errno)),typeid(*this).name(),Log::ERR);

			log.write("Server accepted connection",typeid(*this).name(),Log::DBG);

			// Stocking client address
			data = new ClientData(addrClient, fd);
			data->setThread(new thread(&Server::run,this,data));
			this->lClient->push_back(data);
		}
	}

	// Delete all running instance
	for(vector<ClientData*>::iterator it = this->lClient->begin(); it != this->lClient->end();++it)
	{
		(*it)->stopClient();
		delete *it;
	}

	// TODO Free lAddr
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
		str = &(c->readString());
		if(*str != "")
		{
			log.write("Server ["+ d->getIp() + ":" + d->getPort() + "][RECV] : " + *str, typeid(*this).name(), Log::DBG);
			ServerCmdHandler *cmdHandler = new ServerCmdHandler(*str);

			switch(cmdHandler->exec(d->getFd()))
			{
				case 0: // QUIT
					d->stopClient();
					c->writeString("Bye !");
					log.write("Server finished",typeid(*this).name(),Log::DBG);
					return;
					break;
				case 1:
					c->writeString("OK : " + *str);
					break;
				case -1:
					c->writeString("Command not recognized : " + *str);
					break;
			}

			delete cmdHandler;
		}
	}
}

void Server::stopServer()
{
	this->mt_waitAccept.lock();
	this->waitAccept = false;
	this->mt_waitAccept.unlock();
}
