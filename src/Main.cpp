/*
 * Main file of the SCP Project
 */

#include <iostream>
#include <signal.h>
#include <thread>

#include "iniparseur/INIReader.h"

#include "Log.h"
#include "Server.h"
#include "Client.h"

using namespace std;

void cleanAndStop();

#if (defined DEBUG) && !(defined SERVER || defined CLIENT)

// Configuration file
#define SCP_CONF "./SCP.conf"

// Global access to configuration file
INIReader g_conf(SCP_CONF);

// Global for clean and stop
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
#endif

#ifdef SERVER

// Configuration file
#define CONF_FILE "./server.conf"

// Global access to configuration file
INIReader g_conf(CONF_FILE);

// Global for clean and stop
Server *serv = NULL;
thread *th_serv = NULL;

int main()
{
// Declaration/Initialization
	if(g_conf.ParseError() < 0)
	{
		cout << CONF_FILE << " : Problem with the configuration file" << endl;
		return EXIT_FAILURE;
	}

	Log l(g_conf.Get("Log","LOG_FILE",Log::FILENAME));

	serv = new Server(g_conf.GetInteger("Server","PORT",Server::PORT));

// Run
	// Launch Server
	try
	{
		th_serv = new thread(&Server::acceptConnection,serv);
	}
	catch(system_error &e)
	{
		l << "Launch Server thread";
		l << "What : " << e.what();
		cleanAndStop();
	}
	// Wait for server end
	try
	{
		// TODO !!! Signal on ctrl+C
		serv->stopServer();
		th_serv->join();
		l << "Serv terminated";
	}
	catch(system_error &e)
	{
		l << "Join Server";
		l << "What : " << e.what();
		cleanAndStop();
	}

// Clean
	delete serv;
	delete th_serv;

// End
	return EXIT_SUCCESS;
}
#endif

#ifdef CLIENT

// Configuration file
#define CONF_FILE "./client.conf"

// Global access to configuration file
INIReader g_conf(CONF_FILE);

// Global for clean and stop
Client *client = NULL;
thread *th_client = NULL;

int main()
{
// Declaration/Initialization
	if(g_conf.ParseError() < 0)
	{
		cout << CONF_FILE << " : Problem with the configuration file" << endl;
		return EXIT_FAILURE;
	}

	Log l(g_conf.Get("Log","LOG_FILE",Log::FILENAME));

	client = new Client(g_conf.Get("FirstConnection","IP",Client::HOST),g_conf.GetInteger("FirstConnection","PORT",Client::PORT));

// Run
	// Launch Server
	try
	{
		th_client = new thread(&Client::run,client);
	}
	catch(system_error &e)
	{
		l << "Launch Client thread";
		l << "What : " << e.what();
		cleanAndStop();
	}
	// Wait for server/client end
	try
	{
		th_client ->join();
		l << "Client terminated";
	}
	catch(system_error &e)
	{
		l << "Join Client";
		l << "What : " << e.what();
		cleanAndStop();
	}

// Clean
	delete client;
	delete th_client;

// End
	return EXIT_SUCCESS;
}
#endif

void cleanAndStop()
{
#if (defined DEBUG && !(defined CLIENT)) || ((defined SERVER) && !(defined CLIENT))
	if(serv != NULL)
	{
		serv->stopServer();
		if(th_serv != NULL)
			delete th_serv;

		delete serv;
	}
#endif
#if (defined DEBUG && !(defined SERVER)) || (!(defined SERVER) && (defined CLIENT))
	if(th_client != NULL)
		delete client;
#endif
}
