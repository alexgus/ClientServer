/*
 * Main file of the SCP Project
 */

#include <iostream>

#include "Log.h"
#include "Server.h"
using namespace std;

int main()
{
// Begin
	// Declaration
	int fd;
	Server *serv;

// Test Server
	serv = new Server();
	fd = serv->acceptConnection();
	serv->debug(fd);
	serv->~Server();

// End
	return 0;
}
