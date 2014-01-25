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
void cleanAndStop();

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
	}
	catch(system_error &e)
	{
		cout << "Server thread" << endl;
		cout << "What : " << e.what() << endl;
		cout << "Code : " << e.code() << endl;
		cleanAndStop();
	}
	try
	{
		th_client = new thread(&Client::run,client);
	}
	catch(system_error &e)
	{
		cout << "Client thread" << endl;
		cout << "What : " << e.what() << endl;
		cout << "Code : " << e.code() << endl;
		cleanAndStop();
	}
	try
	{
		th_client ->join();
	}
	catch(system_error &e)
	{
		cout << "Client join" << endl;
		cout << "What : " << e.what() << endl;
		cout << "Code : " << e.code() << endl;
		cleanAndStop();
	}
	try
	{
		th_serv->join();
	}
	catch(system_error &e)
	{
		cout << "Server join" << endl;
		cout << "What : " << e.what() << endl;
		cout << "Code : " << e.code() << endl;
		cleanAndStop();
	}

	delete serv;
	delete th_serv;
	delete client;
	delete th_client;
// End
	return 0;
}

void sigInt(int sig)
{
	cleanAndStop();
}

void cleanAndStop()
{
	if(serv != NULL)
	{
		serv->stopServer();
		if(th_serv != NULL)
		{
			try
			{
				th_serv->join();
			}
			catch(system_error &e)
			{
				cout << "In sigHandler" << endl;
				cout << "What : " << e.what() << endl;
				cout << "Code : " << e.code() << endl;
			}
		}
		delete th_serv;
		delete serv;
	}
	if(th_client != NULL)
		delete client;
}
