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
		cerr << endl << ">>> Can't open Log file !" << endl;
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

void Log::write(string s, string callClass, LEVEL l)
{
	writeTime();

	switch(l)
	{
#ifdef DEBUG
		case DBG:
			file << setw(SETW_LEVEL) << "[DEBUG]";
			break;
#endif
		case WARN:
			file << setw(SETW_LEVEL) << "[WARN]";
			break;
		case ERR:
			file << setw(SETW_LEVEL) << "[ERR]";
			break;
		case INFO:
			file << setw(SETW_LEVEL) << "[INFO]";
			break;
	}
#ifndef DEBUG
	if(l != LEVEL.DBG))
#endif
		file << setw(SETW_CLASS) << "[" + string(&callClass.c_str()[1]) + "]\t" << *this->delNonWantedChar(&s) << endl;
}

void Log::writeTime()
{
	time_t timestamp;
	char buf[20];

	// Get current time
	time(&timestamp);

	// Forlat time
	strftime(buf,20,"%D %T",localtime(&timestamp));
	file << "[" << buf << "] ";
}

string* Log::delNonWantedChar(string* s)
{
	while(s->at(s->size()-1) == '\n'
			|| s->at(s->size()-1) == '\r'
			|| s->at(s->size()-1) == '\0')
		s->erase(s->end()-1);
	return s;
}

