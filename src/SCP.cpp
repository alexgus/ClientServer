/*
 * Main file of the SCP Project
 */

#include <iostream>
#include <signal.h>
#include <thread>

#include "Log.h"
#include "Server.h"
#include "Client.h"
using namespace std;

void cleanAndStop();

Log l;
Server *serv = NULL;
Client *client = NULL;
thread *th_serv = NULL;
thread *th_client = NULL;

int main()
{
// Declaration/Initialization
	serv = new Server();
	client = new Client(HOST,PORT);

// Run
	// Launch Server
	try
	{
		th_serv = new thread(&Server::acceptConnection,serv);
		th_client = new thread(&Client::run,client);
	}
	catch(system_error &e)
	{
		l << "Launch Server/Client thread";
		l << "What : " << e.what();
		cleanAndStop();
	}
	// Wait for server/client end
	try
	{
		th_client ->join();
		l << "Client terminated";
		serv->stopServer();
		th_serv->join();
		l << "Serv terminated";
	}
	catch(system_error &e)
	{
		l << "Join Server/Client";
		l << "What : " << e.what();
		cleanAndStop();
	}

// Clean
	delete serv;
	delete th_serv;
	delete client;
	delete th_client;

// End
	return 0;
}

void cleanAndStop()
{
	if(serv != NULL)
	{
		serv->stopServer();
		if(th_serv != NULL)
			delete th_serv;

		delete serv;
	}
	if(th_client != NULL)
		delete client;
}
