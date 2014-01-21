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

void sigInt(int sig);

Server *serv = NULL;
Client *client = NULL;
thread *th_serv = NULL;
thread *th_client = NULL;

int main()
{
// Begin
	// Declaration
	signal(SIGINT, &sigInt);

// Test Server
	serv = new Server();
	client = new Client(HOST,PORT);

	try
	{
		th_serv = new thread(&Server::acceptConnection,serv);
		sleep(2);
		th_client = new thread(&Client::run,client);
		th_serv->join();
		th_client ->join();
	}
	catch(exception e)
	{
		cout << "inside main : " << e.what() << endl;
	}

	delete serv;
	delete th_serv;
	delete th_client;
// End
	return 0;
}

void sigInt(int sig)
{
	if(serv != NULL)
		delete serv;
	if(th_serv != NULL)
		delete serv;
	if(th_client != NULL)
			delete client;

}
