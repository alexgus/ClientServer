/*
 * Server.cpp
 *
 *  Created on: 11 nov. 2013
 *      Author: alexandre
 */

#include "Server.h"

Server::Server()
{
	log << "Initializing server";

	fd_sock = socket(AF_INET, SOCK_STREAM, 0);

	if(fd_sock == -1){
		log.write("Socket creation error",Log::ERR); // TODO Add errno
		cerr<<"Socket creation error"<<endl;
	}
	addr.sin_family = AF_INET;
	addr.sin_addr.s_addr = htonl(INADDR_ANY); // Conversion big indian
	addr.sin_port = htons(PORT);

	if(bind(fd_sock,(struct sockaddr *) &addr, sizeof(addr)) == -1)
		log.write("Binding error",Log::ERR); // TODO Add errno
		cerr<<"Binding error"<<endl;

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
	int nbRead;
	int i;

	cout << "Debug" << endl;

	while(nbRead == 2) // 2 = '\n' + '\r'
	{
		nbRead = read(fd,buf,TAILLE_BUF);
		if (nbRead == 2) break;

		cout << buf;

		for(i=0;i<TAILLE_BUF;i++)
			buf[i] = '\0'; // FIXME Beuurk

		write(fd, "OK !\n\r", 6); // Acquitement
	}

	close(fd);
}

