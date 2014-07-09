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

	this->mWrite = new mutex();
}

Log::~Log()
{
	// Close file
	this->file.close();
	delete this->mWrite;
}

Log& Log::operator<<(string s)
{
	writeTime();
	this->mWrite->lock();
	file << s << endl;
	this->mWrite->unlock();
	return *this;
}

void Log::write(string s, string callClass, LEVEL l)
{
	writeTime();

	switch(l)
	{
#ifdef DEBUG
		case DBG:
			this->mWrite->lock();
			file << setw(SETW_LEVEL) << "[DEBUG]";
			this->mWrite->unlock();
			break;
#endif
		case WARN:
			this->mWrite->lock();
			file << setw(SETW_LEVEL) << "[WARN]";
			this->mWrite->unlock();
			break;
		case ERR:
			this->mWrite->lock();
			file << setw(SETW_LEVEL) << "[ERR]";
			this->mWrite->unlock();
			break;
		case INFO:
			this->mWrite->lock();
			file << setw(SETW_LEVEL) << "[INFO]";
			this->mWrite->unlock();
			break;
	}
#ifndef DEBUG
	if(l != LEVEL.DBG))
	{
#endif
		this->mWrite->lock();
		file << setw(SETW_CLASS) << "[" + string(&callClass.c_str()[1]) + "]\t" << *this->delNonWantedChar(&s) << endl;
		this->mWrite->unlock();
#ifndef DEBUG
	}
#endif
}

void Log::writeTime()
{
	time_t timestamp;
	char buf[20];

	// Get current time
	time(&timestamp);

	// Forlat time
	strftime(buf,20,"%D %T",localtime(&timestamp));
	this->mWrite->lock();
	file << "[" << buf << "] ";
	this->mWrite->unlock();
}

string* Log::delNonWantedChar(string* s)
{
	while(s->at(s->size()-1) == '\n'
			|| s->at(s->size()-1) == '\r'
			|| s->at(s->size()-1) == '\0')
		s->erase(s->end()-1);
	return s;
}

