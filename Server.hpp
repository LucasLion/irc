
#pragma once

#include "header.hpp"
#include "User.hpp"
#include "Command.hpp"

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
	User				_users[30];

};
