/*
 * Main file of the SCP Project
 */

#include <iostream>
#include <signal.h>

#include "Log.h"
#include "Server.h"
using namespace std;

void sigInt(int sig);

Server *serv = NULL;

int main()
{
// Begin
	// Declaration
	int fd;

	signal(SIGINT, &sigInt);

// Test Server
	serv = new Server();
	fd = serv->acceptConnection();
	serv->run(fd);
	delete serv;

// End
	return 0;
}

void sigInt(int sig)
{
	if(serv != NULL)
		delete serv;
}
