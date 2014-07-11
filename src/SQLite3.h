/*
 * SQLite3.h
 *
 *  Created on: 11 juil. 2014
 *      Author: alexandre
 */

#ifndef SQLITE3_H_
#define SQLITE3_H_

#include <sqlite3.h>

#include <mutex>
#include <string>

using namespace std;

/**
 * Define access to the database SQLite3
 */
class SQLite3
{
public:

	/**
	 * Default constructor
	 * @param File to the database
	 */
	SQLite3(string *db);

	/**
	 * Destroyer of the class
	 * Do nothing cause of static and shared db to not recreate
	 */
	virtual ~SQLite3();

	/**
	 * End connection to the DB
	 * You can next delete the object if you want
	 */
	void end();

	/**
	 * Execute a query to the database
	 * @param sql The sql query
	 * @param callback The callback function to handle restults of the query
	 */
	void exec(string *sql,int (*callback)(void *unused,int nbCol,char** val,char** nameCol));


private:

	/**
	 * Sqlite3 database handler
	 */
	static sqlite3 *db;

	/**
	 * Mutex for protecting execution of request
	 * and respect the data integrity
	 */
	static mutex *mExec;

};

#endif /* SQLITE3_H_ */
