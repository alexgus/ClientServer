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
using namespace std;

#define SETW_LEVEL 7
#define SETW_CLASS 10

class Log
{
private:
	/**
	 * The log's filename
	 */
	string FILENAME = "SCP.log";

	/**
	 * Log's file descriptor
	 */
	ofstream file;

	/**
	 * Write the actual date and time
	 */
	void writeTime();

public:
	/**
	 * Log's level
	 */
	enum LEVEL { DBG = 0, WARN, ERR, INFO };

	/**
	 * Class' constructor
	 */
	Log();

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

#endif /* LOG_H_ */
