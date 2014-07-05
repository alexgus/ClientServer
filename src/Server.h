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
#include <vector>
#include <thread>
#include <mutex>

#include "Log.h"
#include "Com.h"
#include "ServerCmdHandler.h"

using namespace std;

/**
 * Client data
 */
typedef struct
{
	/**
	 * For stocking address' and port's client
	 *
	 * addrClient.sa_data[0] + addrClient.sa_data[1] = PORT client
	 * addrClient.sa_data[5].addrClient.sa_data[4].addrClient.sa_data[3].addrClient.sa_data[2] = IP Client
	 */
	sockaddr *addrClient;

	/**
	 * File descriptor for writing to and reading from the client
	 */
	int fd;

	/**
	 * Client's server thread
	 */
	thread *t;

	/**
	 * Boolean for the main loop of the client
	 */
	bool run;

	/**
	 * Mutex for protecting the boolean run
	 */
	mutex *mRun;
} clientData;

/**
 * Class Server
 */
class Server
{
public:
	/**
	 * The waiting port of the application
	 */
	const static int PORT = 2371;

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
	 * List of clientData
	 */
	vector<clientData*> *lClient;

	/**
	 * While this value is true, the server will wait for new connection.
	 * Protected by mutex.
	 */
	bool waitAccept = true;

	/**
	 * Mutex for protecting waitAccept
	 */
	mutex mt_waitAccept;

	/**
	 * Parse informations sent by the client to this socket.
	 * @param fd File descriptor for communication (socket)
	 */
	void run(clientData *d);

	/**
	 * Stop the communication with the client.
	 * Change properly the state of contRun.
	 */
	void stopRun(clientData *d);

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
