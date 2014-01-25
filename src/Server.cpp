/*
 * Server.cpp
 *
 *  Created on: 11 nov. 2013
 *      Author: Alexandre Guyon
 */

#include "Server.h"

Server::Server()
{
#ifdef DEBUG
		log.write("Initialize Server",Log::DBG);
#endif
	// Create the socket
	fd_sock = socket(AF_INET, SOCK_STREAM, 0);
	if(fd_sock == -1)
		log.write("Socket creation error : " + string(strerror(errno)),Log::ERR);

	int optVal =1;
	if(setsockopt(fd_sock,SOL_SOCKET,SO_REUSEADDR,&optVal,sizeof(int)) == -1)
	{
		log.write("Server : Failed to change socket option : " + string(strerror(errno)),Log::ERR);
	}

	// Bind the socket to the system
	addr.sin_family = AF_INET;
	addr.sin_addr.s_addr = htonl(INADDR_ANY); // Conversion big indian
	addr.sin_port = htons(PORT);
	if(bind(fd_sock,(struct sockaddr *) &addr, sizeof(addr)) == -1)
		log.write("Binding error : " + string(strerror(errno)),Log::ERR);

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

	sockaddr addrClient;
	socklen_t lenAddrClient;
	list<thread> lClient;

	addrClient.sa_family = AF_INET;
	lenAddrClient = sizeof(addrClient);

// Begin
	// If no socket
	if(fd_sock < 0)
		return;

#ifdef DEBUG
		log.write("Server wait for connection",Log::DBG);
#endif

	while(this->waitAccept)
	{
		// Set the fd_set
		ret_select = 0;
		timeoutAccept = { timeout , 0 };
		FD_ZERO(&read);
		FD_SET(fd_sock, &read);

		if((ret_select = select(fd_sock+1,&read,NULL,NULL,&timeoutAccept)) < 0)
		{
			log.write("Select accept error : " + string(strerror(errno)),Log::ERR);
			return;
		}

		if(ret_select > 0)
		{
			fd = accept(fd_sock, &addrClient, &lenAddrClient);
			if(fd < 0)
				log.write("Accept error : " + string(strerror(errno)),Log::ERR);

#ifdef DEBUG
			log.write("Server accepted connection",Log::DBG);
#endif
			lClient.push_back(thread(&Server::run,this,fd));
		}
	}

	this->stopRun();
	// Delete all run instance
	for(list<thread>::iterator it=lClient.begin();it != lClient.end();++it)
		it->join();
}

void Server::run(int fd)
{
	int ret_select;
	fd_set readSet;
	timeval timeoutRead;

	char buf[TAILLE_BUF];
	int nbRead = 0;

	// If fd isn't file descriptor
	if(fd < 0)
		return;

	while(this->contRun)
	{
		ret_select = 0;
		timeoutRead = { timeout , 0 };
		FD_ZERO(&readSet);
		FD_SET(fd, &readSet);

		if((ret_select = select(fd+1,&readSet,NULL,NULL,&timeoutRead)) < 0)
		{
			log.write("Select read error : " + string(strerror(errno)),Log::ERR);
			return;
		}
		// the state of fd changed
		if(ret_select > 0)
		{
			// Read command
			nbRead = read(fd,buf,TAILLE_BUF);
			buf[nbRead] = '\0'; // Add end of string

			string *str = new string(buf);
			ServerCmdHandler *cmdHandler = new ServerCmdHandler(*str);
			cmdHandler->exec();
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
