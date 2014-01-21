/*
 * Main file of the SCP Project
 */

#include <iostream>
#include <signal.h>
#include <thread>

#include "Log.h"
#include "Server.h"
using namespace std;

void sigInt(int sig);

Server *serv = NULL;
thread *th_serv = NULL;

int main()
{
// Begin
	// Declaration
	signal(SIGINT, &sigInt);

// Test Server
	serv = new Server();

	try
	{
		th_serv = new thread(&Server::acceptConnection,serv);
		th_serv->join();
	}
	catch(exception e)
	{
		cout << "inside main : " << e.what() << endl;
	}

	delete serv;
	delete th_serv;
// End
	return 0;
}

void sigInt(int sig)
{
	if(serv != NULL)
		delete serv;
	if(th_serv != NULL)
		delete serv;
}
