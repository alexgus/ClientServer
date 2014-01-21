/*
 * Client.cpp
 *
 *  Created on: 20 Jan 2014
 *      Author: nicolas
 */

#include "Client.h"

Client::Client(){
init();
}

Client::Client(char* address, char* port){
		init();
		initHostInfo(address, port);


}

int Client::initHostInfo( char*address, char* port){
	status = getaddrinfo(address, port, &host_info, &host_info_list);
		 if(status != 0){
			log<<"Client : Failed to get Hosts Info";
			return status;
		 }

		log<<"Client is ready to connect to the server";
		return status;
}

int Client::connection(){
	fd_sock = socket(host_info_list->ai_family,
			host_info_list->ai_socktype,
			host_info_list->ai_protocol);

		 if (fd_sock == -1) cout<<"Client : Failed creating the socket\n";

		 status = connect(fd_sock, host_info_list->ai_addr, host_info_list->ai_addrlen);
		 if (status == -1) log<<"Client : Connection error : Failed to connect to Host";

		 return status;
}
void Client::run(){
	connection();
	for(;;){
sleep(1);
	}
}
void Client::init(){
		fd_sock = -1;
		status = -1;
		memset(&host_info, 0, sizeof host_info);
		host_info.ai_family = AF_UNSPEC;     // IP version not specified. Can be both.
		host_info.ai_socktype = SOCK_STREAM; // Use SOCK_STREAM for TCP or SOCK_DGRAM for UDP.
}

Client::~Client()
{

}
