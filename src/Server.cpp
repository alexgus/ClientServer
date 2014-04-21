/*
 * Server.cpp
 *
 *  Created on: 11 nov. 2013
 *      Author: Alexandre Guyon
 */

#include "Server.h"

Server::Server()
{
	log.write("Initialize Server",Log::DBG,typeid(this).name());

	// Create the socket
	fd_sock = socket(AF_INET, SOCK_STREAM, 0);
	if(fd_sock == -1)
		log.write("Socket creation error : " + string(strerror(errno)),Log::ERR,typeid(this).name());

	int optVal =1;
	if(setsockopt(fd_sock,SOL_SOCKET,SO_REUSEADDR,&optVal,sizeof(int)) == -1)
		log.write("Server : Failed to change socket option : " + string(strerror(errno)),Log::ERR,typeid(this).name());

	// Bind the socket to the system
	addr.sin_family = AF_INET;
	addr.sin_addr.s_addr = htonl(INADDR_ANY); // Conversion big indian
	addr.sin_port = htons(PORT);
	if(bind(fd_sock,(struct sockaddr *) &addr, sizeof(addr)) == -1)
		log.write("Binding error : " + string(strerror(errno)),Log::ERR,typeid(this).name());

	// Set the listen queue
	listen(fd_sock, 5); // 5 = limit size of the connection queue
}

Server::~Server()
{
	if(fd_sock != -1)
		close(fd_sock);
}


void Server::acceptConnection()
{
// Declaration
	int fd;
	int ret_select;
	fd_set read;
	timeval timeoutAccept;

	/*
	 * addrClient.sa_data[0] + addrClient.sa_data[1] = PORT client
	 * addrClient.sa_data[5].addrClient.sa_data[4].addrClient.sa_data[3].addrClient.sa_data[2] = IP Client
	 */
	sockaddr *addrClient;
	socklen_t lenAddrClient = sizeof(sockaddr);;
	list<thread> lClient;
	list<sockaddr> lAddr;

// Begin
	// If no socket
	if(fd_sock < 0)
		return;

	log.write("Server wait for connection",Log::DBG);

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
			log.write("Select accept error : " + string(strerror(errno)),Log::ERR,typeid(this).name());
			return;
		}

		if(ret_select > 0)
		{
			addrClient = (sockaddr*) malloc(lenAddrClient);
			fd = accept(fd_sock, addrClient, &lenAddrClient);

			if(fd < 0)
				log.write("Accept error : " + string(strerror(errno)),Log::ERR,typeid(this).name());

			log.write("Server accepted connection",Log::DBG,typeid(this).name());

			lAddr.push_back(*addrClient);
			lClient.push_back(thread(&Server::run,this,fd));
		}
	}

	this->stopRun();
	// Delete all run instance
	for(list<thread>::iterator it=lClient.begin();it != lClient.end();++it)
		it->join();

	// TODO Free lAddr
}

void Server::run(int fd)
{
	// If fd isn't file descriptor
	if(fd < 0)
		return;

	Com *c = new Com(fd,{timeoutS,timeoutM});
	string *str;

	this->mt_contRun.lock();
	while(this->contRun)
	{
		this->mt_contRun.unlock();
		if((str = &(c->readString())) != NULL)
		{
			ServerCmdHandler *cmdHandler = new ServerCmdHandler(*str);

			switch(cmdHandler->exec(fd))
			{
				case 0: // QUIT
					this->stopRun();
					c->writeString("Bye !");
					log.write("Server finished",Log::DBG,typeid(this).name());
					return;
					break;
				case 1:
					c->writeString("OK : " + *str);
					break;
				case -1:
					c->writeString("Command not recognized : " + *str);
					break;
			}
		}
	}

	// Close the file descriptor
	close(fd);
}

void Server::stopServer()
{
	this->mt_waitAccept.lock();
	this->waitAccept = false;
	this->mt_waitAccept.unlock();
}

void Server::stopRun()
{
	this->mt_contRun.lock();
	this->contRun = false;
	this->mt_contRun.unlock();
}
