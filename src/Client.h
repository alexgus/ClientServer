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
#include <thread>
#include <mutex>

#include "Log.h"
#include "Com.h"

using namespace std;

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
	 * Mutex for protect running variable
	 */
	mutex *mRunning;

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
	 * Method for listening on the socket
	 */
	void listenOn();

	/**
	 * Method for writing command to the socket
	 */
	void writeCommand();

	/**
	 * Connect to the host
	 * @return The status of the connection
	 */
	int connection();

	/**
	 * Get the client architecture
	 */
	string* getArch();

public :

	/**
	 * Default port of the client will connect
	 */
	static const int PORT;

	/**
	 * Default host of the client will connect
	 */
	static const string HOST;

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

const int Client::PORT = 2371;
const string Client::HOST = "127.0.0.1";

#endif /* CLIENT_H_ */
