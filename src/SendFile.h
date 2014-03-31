/*
 * SendFile.h
 *
 *  Created on: 27 janv. 2014
 *      Author: alexandre
 */

#ifndef SENDFILE_H_
#define SENDFILE_H_

#include <string>
#include <fstream>
#include <typeinfo>

#include "Log.h"
#include "Com.h"

using namespace std;

#define MAX_MTU 1024

/**
 * Class for sending a file to a client
 */
class SendFile
{
public:
	/**
	 * Default constructor
	 */
	SendFile();

	/**
	 * Constructors of the class.
	 * @param path The path to the file
	 * @param portClient The client port to send the file
	 */
	SendFile(string path, int clientPort);

	/**
	 * Destroyer of the class
	 */
	virtual ~SendFile();

	/**
	 * Function for sending the file
	 */
	void send();

private:
	/**
	 * Log file
	 */
	Log log;

	/**
	 * Communication methods
	 */
	Com *com = NULL;

	/**
	 * File to send
	 */
	ifstream file;
};

#endif /* SENDFILE_H_ */
