
#pragma once

#include "header.hpp"
#include "Channel.hpp"


class Server {

public:

						Server( void );
						Server( char *port, char *passwd );
	int					createSocket( void );
	void				handleConnections( void );
	void				newConnection( void );
	void				loop( void );
	bool				createChannel( std::string name );

private:
	
	int								_maxClients;
	int								_portno;
	std::vector<int>				_clientSockets;
	struct sockaddr_in				_address;
	int								_addrLen;
	int								_masterSocket;
	fd_set							_readfds;
	std::string						_buffer;
	int								_max_sd;
	std::string						_passwd;
	std::map<std::string, Channel>	_channels;
	User							_users[30];
};
