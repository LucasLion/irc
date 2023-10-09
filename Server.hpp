
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
//#include "Command.hpp"
#include "User.hpp"

class Server {

public:

						Server( void );
						Server( char *port, char *passwd );
	int					createSocket( void );
	void				handleConnections( void );
	void				newConnection( void );
	void				loop( void );

private:
	
	int					_maxClients;
	int					_portno;
	std::vector<int>	_clientSockets;
	struct sockaddr_in	_address;
	int					_addrLen;
	int					_masterSocket;
	fd_set				_readfds;
	//char				_buffer[1025];
	std::string			_buffer;
	int					_max_sd;
	std::string			_passwd;
	std::vector<User>	_users;

};
