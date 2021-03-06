/*
 * Com.h
 *
 *  Created on: 25 janv. 2014
 *      Author: alexandre
 */

#ifndef COM_H_
#define COM_H_

#include <time.h>
#include <sys/select.h>
#include <unistd.h>
#include <string>
#include <string.h>
#include <errno.h>
#include <typeinfo>

#include "Log.h"

#define BUFFER_SIZE 512

using namespace std;

class Com
{
public:
	/**
	 * Constructor of the class
	 */
	Com();

	/**
	 * Constructor of the class
	 * @param fd The file descriptor to read/write
	 * @param timeOut The time out for the read operation
	 */
	Com(int fd, timeval timeOut);

	/**
	 * Destroyer of the class
	 */
	virtual ~Com();

	/**
	 * Write the string to the fd
	 * @param s String to write
	 */
	void writeString(const string &s);

	/**
	 * Read a string from the fd
	 * @return The string read
	 */
	string* readString();

	/**
	 * Read byte on the file descriptor
	 * @param The buffer to write
	 * @param size Size to read. By default = BUFFER_SIZE
	 */
	void writeBlob(void* buf, int size = BUFFER_SIZE);

	/**
	 * Read byte on the file descriptor
	 * @param size Size to read. By default = BUFFER_SIZE
	 * @return The data read
	 */
	char* readBlob(int size = BUFFER_SIZE);

	/**
	 * Get the file descriptor associate with this object
	 * @return The file descriptor
	 */
	int getFd() const
	{
		return fd;
	}

private:
	/**
	 * File descriptor to read/write
	 */
	int fd;

	/**
	 * The timeout for the read operation
	 */
	timeval timeout;

	/**
	 * The log file
	 */
	Log log;

	/**
	 * Delete last \0 \r \n in a string
	 */
	string* delNonWantedChar(string *s);

};

#endif /* COM_H_ */
