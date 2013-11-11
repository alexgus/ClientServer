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
#include <thread>
#include <string>

#include "Log.h"


#include <iostream>
using namespace std;


#define TAILLE_BUF 256

class Server
{
private:
	Log log;

	int PORT = 2348;
	int fd_sock;
	struct sockaddr_in addr;

	list<sockaddr_in> listClient;

public:
	Server();
	virtual ~Server();

	int acceptConnection();
	void debug(int fd);
};

#endif /* SERVER_H_ */
