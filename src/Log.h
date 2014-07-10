/*
 * Log.h
 *
 *  Created on: 11 nov. 2013
 *      Author: alexgus
 *
 * Allow to log all action into a file with
 * various log level.
 */

#ifndef LOG_H_
#define LOG_H_

#include <iostream>
#include <fstream>
#include <string>
#include <time.h>
#include <iomanip>
#include <mutex>
using namespace std;

#define SETW_LEVEL 7
#define SETW_CLASS 10

class Log
{
private:

	/**
	 * Log's file descriptor
	 */
	ofstream file;

	/**
	 * Mutex for protecting the file from threads
	 */
	mutex *mWrite;

	/**
	 * Write the actual date and time
	 */
	void writeTime();

	/**
	 * Delete last \0 \r \n in a string
	 */
	string* delNonWantedChar(string *s);

public:
	/**
	 * The log's filename
	 */
	static const string FILENAME;

	/**
	 * Log's level
	 */
	enum LEVEL { DBG = 0, WARN, ERR, INFO };

	/**
	 * Class' constructor
	 */
	Log();

	/**
	 * Class' constructor
	 * @param file Log file
	 */
	Log(string filename);

	/**
	 * Class' destructor
	 */
	virtual ~Log();

	/**
	 * Write the string to the log with the level l
	 * @param s The string to log
	 * @param l The log level
	 * @param callClass The name of the class who called the function
	 */
	void write(string s, string callClass, LEVEL l);

	/**
	 * Write to the log without log level
	 * @param s The string to log
	 */
	Log& operator<<(string s);
};

const string Log::FILENAME = "SCP.log";

#endif /* LOG_H_ */
