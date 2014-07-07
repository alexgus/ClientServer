/*
 * ClientData.h
 *
 *  Created on: 6 juil. 2014
 *      Author: alexandre
 */

#ifndef CLIENTDATA_H_
#define CLIENTDATA_H_

#include <netinet/in.h>
#include <unistd.h>
#include <math.h>

#include <thread>
#include <mutex>
#include <iostream>

using namespace std;

/**
 * Group all data about a client (ip, port, thread, ...)
 */
class ClientData
{
private:
	/**
	 * For stocking address' and port's client
	 *
	 * addrClient.sa_data[0] + addrClient.sa_data[1] = PORT client
	 * addrClient.sa_data[5].addrClient.sa_data[4].addrClient.sa_data[3].addrClient.sa_data[2] = IP Client
	 */
	sockaddr *addrClient;

	/**
	 * String Ip address
	 */
	string *ip;

	/**
	 * String port address
	 */
	string *port;

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

public:

	/**
	 * Default constructor. Construct a client with all needed information
	 */
	ClientData(sockaddr* s, int fd);

	/**
	 * Default destroyer
	 */
	virtual ~ClientData();

	/**
	 * Stop the client thread
	 */
	void stopClient();

	/**
	 * Get the file descriptor for talking with the client
	 * @return The file descriptor
	 */
	int getFd() const
	{
		return fd;
	}

	/**
	 * Check if the client is currently running
	 * @return
	 */
	bool isRunning() const
	{
		return run;
	}

	/**
	 * Get the thread of the client
	 * @return The client's thread
	 */
	thread* getThread() const
	{
		return t;
	}

	/**
	 * Set the client's thread
	 * @param t The client's thread to set
	 */
	void setThread(thread* t)
	{
		this->t = t;
	}

	/**
	 * Lock the client mutex
	 */
	void lockRun() const
	{
		this->mRun->lock();
	}

	/**
	 * Unlock the client mutex
	 */
	void unlockRun()
	{
		this->mRun->unlock();
	}

	string* getIp() const
	{
		return ip;
	}

	string* getPort() const
	{
		return port;
	}
};

#endif /* CLIENTDATA_H_ */
