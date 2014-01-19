/*
 * Server.h
 *
 *  Created on: 11 nov. 2013
 *      Author: alexandre
 */

#ifndef SERVER_H_
#define SERVER_H_

#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <list>
#include <string>
#include "Log.h"

using namespace std;

#define TAILLE_BUF 256


class Server
{
private:
	/**
	 * For display informations
	 */
	Log log;

	/**
	 * The wait port of the application
	 */
	int PORT = 2357;

	/**
	 * Wait socket
	 */
	int fd_sock;

	/**
	 * Informations about the current address of this server
	 */
	struct sockaddr_in addr;

	/**
	 * List of connected clients
	 */
	list<sockaddr_in> listClient;

public:
	Server();
	virtual ~Server();

	int acceptConnection();
	void debug(int fd);
};

#endif /* SERVER_H_ */
