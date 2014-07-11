/*
 * SQLite3.cpp
 *
 *  Created on: 11 juil. 2014
 *      Author: alexandre
 */

#include "SQLite3.h"

sqlite3 *SQLite3::db = 0;
mutex *SQLite3::mExec = 0;

SQLite3::SQLite3(string* db)
{
	if(SQLite3::db == 0)
	{
		sqlite3_initialize();
		sqlite3_open(db->c_str(),&SQLite3::db);
		SQLite3::mExec = new mutex();
	}
}

SQLite3::~SQLite3()
{

}

void SQLite3::end()
{
	SQLite3::mExec->lock();
	sqlite3_shutdown();
	sqlite3_close(SQLite3::db);
	SQLite3::mExec->unlock();
	delete SQLite3::mExec;
}

void SQLite3::exec(string* sql,
		int (*callback)(void* unused, int nbCol, char** val, char** nameCol))
{
	SQLite3::mExec->lock();
	sqlite3_exec(SQLite3::db,sql->c_str(),callback,0,0);
	SQLite3::mExec->unlock();
}
