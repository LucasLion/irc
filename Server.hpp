
#pragma once

#include <iostream>
#include <string>
#include <strings.h>
#include <string.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <sys/time.h>
#include <unistd.h>
#include <stdlib.h>
#include <cstdio>
#include <cerrno>
#include <vector>

class Server {

public:

						Server( void );
						Server( char *port);
	int					createSocket();
	void				handle_connections();
	void				new_connection();
	void				loop();

private:
	
	int					_maxClients;
	int					_portno;
	std::vector<int>	_clientSockets;
	struct sockaddr_in	_address;
	int					_addrLen;
	int					_masterSocket;
	fd_set				_readfds;
	char				buffer[1025];
	int					_max_sd;

};
