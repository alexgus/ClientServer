/*
 * Client.h
 *
 *  Created on: 20 Jan 2014
 *      Author: nicolas
 */

#ifndef CLIENT_H_
#define CLIENT_H_



#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <typeinfo>
#include <cstring>
#include <iostream>
#include <string>
#include <netdb.h>
#include <stdlib.h>
#include <sys/statvfs.h>
#include "Log.h"
#include "Com.h"

using namespace std;

//Target host details:
#define PORT  2371
#define HOST "127.0.0.1"

class Client
{
private :

	Log log;
	int fd_sock;
	struct sockaddr_in addr;
	int status;
	struct addrinfo host_info;       // The struct that getaddrinfo() fills up with data.
	struct addrinfo *host_info_list; // Pointer to the to the linked list of host_info's.
	void init();
	int connection();
	int sendCmd(string cmd);
	string receive();
public :

	Client();
	Client(string address, int port);
	virtual ~Client();

	int initHostInfo(string address, int port);
	void run();
};


#endif /* CLIENT_H_ */
