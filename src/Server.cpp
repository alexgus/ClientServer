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

	this->lClient = new vector<clientData*>();

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

	clientData *data;

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
			data = (clientData *)malloc(sizeof(clientData));
			data->addrClient = addrClient;
			data->fd = fd;
			data->mRun = new mutex();
			data->run = true;
			data->t = new thread(&Server::run,this,data);
			this->lClient->push_back(data);
		}
	}

	// Delete all running instance
	for(vector<clientData*>::iterator it = this->lClient->begin(); it != this->lClient->end();++it)
	{
		this->stopRun(*it);
		(*it)->t->join();
		free(*it);
	}

	// TODO Free lAddr
}

void Server::run(clientData *d)
{
	// If fd isn't file descriptor
	if(d->fd < 0)
		return;

	char *ipPort = (char*) malloc(sizeof(char)*24);
	snprintf(ipPort,24, "%hd.%hd.%hd.%hd:%d", d->addrClient->sa_data[2], d->addrClient->sa_data[3], d->addrClient->sa_data[4], d->addrClient->sa_data[5],
			(d->addrClient->sa_data[0] + d->addrClient->sa_data[1]));

	Com *c = new Com(d->fd,{timeoutS,timeoutM});
	string *str;

	d->mRun->lock();
	while(d->run)
	{
		d->mRun->unlock();
		str = &(c->readString());
		if(*str != "")
		{
			log.write("Server ["+ string(ipPort) + "][RECV] : " + *str, typeid(*this).name(), Log::DBG);
			ServerCmdHandler *cmdHandler = new ServerCmdHandler(*str);

			switch(cmdHandler->exec(d->fd))
			{
				case 0: // QUIT
					this->stopRun(d);
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

	free(ipPort);

	// Close the file descriptor
	close(d->fd);
}

void Server::stopServer()
{
	this->mt_waitAccept.lock();
	this->waitAccept = false;
	this->mt_waitAccept.unlock();
}

void Server::stopRun(clientData *d)
{
	d->mRun->lock();
	d->run = false;
	d->mRun->unlock();
}
