/*
 * Client.h
 *
 *  Created on: 20 Jan 2014
 *      Author: nicolas
 */

#ifndef CLIENT_H_
#define CLIENT_H_

#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <stdlib.h>

#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <netdb.h>

#include <sys/statvfs.h>

#include <typeinfo>
#include <cstring>
#include <iostream>
#include <string>

#include "Log.h"
#include "Com.h"

using namespace std;

//Target host details:
#define PORT  2371
#define HOST "127.0.0.1"

class Client
{
private :

	/**
	 * For logging event
	 */
	Log log;

	/**
	 * If the client is running
	 */
	bool running;

	/**
	 * The socket file descriptor
	 */
	int fd_sock;

	/**
	 * The client's address
	 */
	struct sockaddr_in addr;

	/**
	 * The struct that getaddrinfo() fills up with data.
	 */
	struct addrinfo host_info;

	/**
	 * Pointer to the to the linked list of host_info's.
	 */
	struct addrinfo *host_info_list;

	/**
	 * Communication set of method for the user
	 */
	Com *cUser;

	/**
	 * Communication set of method for the socket (server)
	 */
	Com *cSocket;

	/**
	 * Connect to the host
	 * @return The status of the connection
	 */
	int connection();

public :

	/**
	 * Default constructor
	 */
	Client();

	/**
	 * Constructor defines address and port to connect
	 * @param address The address to connect
	 * @param port The port to connect
	 */
	Client(string address, int port);

	/**
	 * Default destroyer
	 */
	virtual ~Client();

	/**
	 * Initialize the connection
	 * @param address The address to connect
	 * @param port The port to connect
	 * @return The status of the host
	 */
	int initHostInfo(string address, int port);

	/**
	 * Run the client
	 */
	void run();
};


#endif /* CLIENT_H_ */
