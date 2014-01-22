/*
 * Log.cpp
 *
 *  Created on: 11 nov. 2013
 *      Author: alexandre
 */

#include "Log.h"

Log::Log()
{
	// Opening in write-only and append to the next
	this->file.open(FILENAME.c_str(), std::ofstream::out | std::ofstream::app);

	// If the file isn't open, display an error on cerr
	if(!file.is_open())
	{
		cerr << endl << ">>> Can't open Log file !" << endl;
	}
}

Log::~Log()
{
	// Close file
	this->file.close();
}

Log& Log::operator<<(string s)
{
	writeTime();
	file << s << endl;
	return *this;
}

void Log::write(string s, LEVEL l)
{
	writeTime();

	switch(l)
	{
		case DBG:
			file << "\t[DEBUG]\t";
			break;
		case WARN:
			file << "\t[WARN]\t";
			break;
		case ERR:
			file << "\t[ERR]\t";
			break;
	}

	file << s << endl;
}

void Log::writeTime()
{
	time_t timestamp;
	struct tm* local;

	// Get current time
	time(&timestamp);
	local = localtime(&timestamp); // Convert to struct tm

	file << "[" << local->tm_year + 1900 << "/" << local->tm_mon+1 << "/" << local->tm_mday << " " << local->tm_hour << ":" << local->tm_min << ":" << local->tm_sec << "] ";
}


