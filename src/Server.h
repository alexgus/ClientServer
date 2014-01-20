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

#include "Log.h"

using namespace std;

#define TAILLE_BUF 256

// TODO Add exception

class Server
{
public:
	/**
	 * Get an element
	 */
	const string CMD_GET = string("GET\r\n");

	/**
	 * Put an element
	 */
	const string CMD_PUT = string("PUT\r\n");

	/**
	 * Put an element
	 */
	const string CMD_QUIT = string("\r\n");

	/**
	 * The wait port of the application
	 */
	const int PORT = 2370;

private:
	/**
	 * For display informations
	 */
	Log log;

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
	/**
	 * Constructor of the class.
	 * Initialize a socket
	 */
	Server();

	/**
	 * Destroyer of the class.
	 * Delete the socket
	 */
	virtual ~Server();

	/**
	 * Wait for a client.
	 * @return The file descriptor for writing to the accepted client
	 */
	int acceptConnection();

	/**
	 * Parse informations sent by the client
	 * @param fd File descriptor for communication
	 */
	void run(int fd);
};

#endif /* SERVER_H_ */
