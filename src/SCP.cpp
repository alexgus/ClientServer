/*
 * Main file of the SCP Project
 */

#include <iostream>
#include <signal.h>
#include <thread>

#include "iniparseur/INIReader.h"
#define SCP_CONF "./SCP.conf"

INIReader g_conf(SCP_CONF);

#include "Log.h"
#include "Server.h"
#include "Client.h"

using namespace std;

void cleanAndStop();

Server *serv = NULL;
Client *client = NULL;
thread *th_serv = NULL;
thread *th_client = NULL;

int main()
{
// Declaration/Initialization
	if(g_conf.ParseError() < 0)
	{
		cout << SCP_CONF << " : Problem with the configuration file" << endl;
		return EXIT_FAILURE;
	}

	Log l(g_conf.Get("Log","LOG_FILE",Log::FILENAME));

	serv = new Server(g_conf.GetInteger("Server","PORT",Server::PORT));
	client = new Client(g_conf.Get("FirstConnection","IP",Client::HOST),g_conf.GetInteger("FirstConnection","PORT",Client::PORT));

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
	return EXIT_SUCCESS;
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
