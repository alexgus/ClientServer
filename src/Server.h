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
#include <sys/select.h>

#include <netinet/in.h>
#include <unistd.h>
#include <time.h>
#include <string.h>

#include <cerrno>
#include <typeinfo>
#include <string>
#include <list>
#include <thread>
#include <mutex>

#include "Log.h"
#include "Com.h"
#include "ServerCmdHandler.h"

using namespace std;

/**
 * Class Server
 */
class Server
{
public:
	/**
	 * The waiting port of the application
	 */
	const static int PORT = 2370;

	/**
	 * Timeout of select (in seconds)
	 */
	const static int timeoutS = 2;

	/**
	 * Timeout of select (in milliseconds)
	 */
	const static int timeoutM = 0;

private:
	/**
	 * Log object for display informations
	 * about the server.
	 */
	Log log;

	/**
	 * Wait socket of the server.
	 * Can accept connection on this file descriptor.
	 */
	int fd_sock;

	/**
	 * Informations about the current address
	 * and port of this server.
	 */
	struct sockaddr_in addr;

	/**
	 * While this value is true, the server will wait for new connection.
	 * Protected by mutex.
	 */
	bool waitAccept = true;

	/**
	 * Continue to talk with a client.
	 * Protected by mutex.
	 */
	bool contRun = true;

	/**
	 * Mutex for protecting waitAccept
	 */
	mutex mt_waitAccept;

	/**
	 * Mutex for protecting contRun
	 */
	mutex mt_contRun;

	/**
	 * Parse informations sent by the client to this socket.
	 * @param fd File descriptor for communication (socket)
	 */
	void run(int fd);

	/**
	 * Stop the communication with the client.
	 * Change properly the state of contRun.
	 */
	void stopRun();

public:
	/**
	 * Constructor of the class.
	 * Initialize a socket with a port and an address.
	 */
	Server();

	/**
	 * Destroyer of the class.
	 * Delete the socket and stop connection with clients
	 */
	virtual ~Server();

	/**
	 * Wait for a client to be connect on the waiting port.
	 * It launch next the parser (function run) in a thread
	 * for communicating with the client.
	 * Record too the thread with address and port of the client
	 * in a list.
	 */
	void acceptConnection();

	/**
	 * Stop the thread server. Stop Connection waiting.
	 */
	void stopServer();
};

#endif /* SERVER_H_ */
