/*
 * Server.cpp
 *
 *  Created on: 11 nov. 2013
 *      Author: Alexandre Guyon
 */

#include "Server.h"

Server::Server()
{
	log << "Initializing server";

	fd_sock = socket(AF_INET, SOCK_STREAM, 0);

	if(fd_sock == -1){
		log.write("Socket creation error : " + string(strerror(errno)),Log::ERR);
		cerr<<"Socket creation error"<<endl;
	}
	addr.sin_family = AF_INET;
	addr.sin_addr.s_addr = htonl(INADDR_ANY); // Conversion big indian
	addr.sin_port = htons(PORT);

	if(bind(fd_sock,(struct sockaddr *) &addr, sizeof(addr)) == -1)
		log.write("Binding error : " + string(strerror(errno)),Log::ERR);

	listen(fd_sock, 5); // 5 = limit size of the connection queue
}

Server::~Server()
{
	if(fd_sock != -1)
		close(fd_sock);
	// TODO unlink
}

int Server::acceptConnection()
{
	sockaddr a;
	socklen_t tmp;
	int fd;

	// If no socket
	if(fd_sock < 0)
		return -1;

	a.sa_family = AF_INET;
	tmp = sizeof(a);
	cout << "Wait accept" << endl;
	fd = accept(fd_sock, &a, &tmp);
	if(fd==-1)
		perror("accept");

	// TODO Add client to listClient
	log << "Connection accepted from ****"; // TODO Finish log here

	return fd;
}

void Server::debug(int fd)
{
	char buf[TAILLE_BUF];
	int nbRead = 0;

	// If fd isn't file descriptor
	if(fd < 0)
		return;

	while(nbRead != 2) // 2 = '\n' + '\r'
	{
		nbRead = read(fd,buf,TAILLE_BUF);
		buf[nbRead] = '\0';

		// Debug
		cout << buf;

		if(string(buf) == CMD_GET)
		{
			write(fd,(CMD_GET+"\n\r").c_str(),CMD_GET.size()+2);
		}
		else if(string(buf) == CMD_PUT)
		{
			write(fd,(CMD_PUT+"\n\r").c_str(),CMD_PUT.size()+2);
		}
		else
		{
			write(fd, "OK !\n\r", 6); // Ack
		}
	}

	close(fd);
}

