/*
 * Server.cpp
 *
 *  Created on: 11 nov. 2013
 *      Author: Alexandre Guyon
 */

#include "Server.h"
//
Server::Server()
{
	log << "Initializing server";

	// Create the socket
	fd_sock = socket(AF_INET, SOCK_STREAM, 0);
	if(fd_sock == -1)
		log.write("Socket creation error : " + string(strerror(errno)),Log::ERR);

	/*int optVal = 0x1;
	if(setsockopt(fd_sock,IPPROTO_TCP,SO_REUSEADDR,&optVal,sizeof(int)) == -1)
	{
		log.write("Set SO_REUSEADDR error : " + string(strerror(errno)),Log::ERR);
	}*/

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
	sockaddr addrClient;
	socklen_t lenAddrClient;
	thread run;
	int fd;

	// If no socket
	if(fd_sock < 0)
		return;

	while(1)
	{
		addrClient.sa_family = AF_INET;
		lenAddrClient = sizeof(addrClient);
		fd = accept(fd_sock, &addrClient, &lenAddrClient);
		if(fd < 0)
			log.write("Accept error : " + string(strerror(errno)),Log::ERR);

		log << "Connection accepted";


		run = thread(&Server::run,this,fd);
		//this->run(fd);
	}
	run.join();
}

// TODO add process/thread
void Server::run(int fd)
{
	char buf[TAILLE_BUF];
	int nbRead = 0;
	bool cont = true;

	// If fd isn't file descriptor
	if(fd < 0)
		return;

	while(cont)
	{
		// Read command
		nbRead = read(fd,buf,TAILLE_BUF);
		buf[nbRead] = '\0'; // Add end of string

		// Find the command
		if(string(buf) == CMD_GET)
		{
			write(fd,(CMD_GET).c_str(),CMD_GET.size());
		}
		else if(string(buf) == CMD_PUT)
		{
			write(fd,(CMD_PUT).c_str(),CMD_PUT.size());
		}
		else if(string(buf) == CMD_QUIT)
		{
			write(fd, "Bye !\n\r", 6);
			cont = false;
		}
		else // Doesn't find any commands
			write(fd, "OK ! What do you want ?\n\r", 25);
	}

	// Close the file descriptor
	close(fd);
}

