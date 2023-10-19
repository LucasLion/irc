
#pragma once

#include "header.hpp"
#include "Channel.hpp"
#include "User.hpp"

class Server {

public:

						Server( void );
						Server( char *port, char *passwd );
	int					createSocket( void );
	void				handleConnections( void );
	void				newConnection( void );
    void                getBuffer( char * buf );
	void				run( void );
	bool				createChannel( std::string name );
	bool				generateResponse( User *user );
	void				userCmd( Message msg, User *user );
	void				nickCmd( Message msg, User *user );
	void				passCmd( Message msg, User *user );
	void				joinCmd( Message msg, User *user );
	void				pongCmd( Message msg, User *user );
	int					getPortno( void ) const;


	std::map<std::string, Channel>	getChannels( void ) const;

private:
	
	std::string						_name;
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
    std::vector<Message>		    _messages;
	// rajouter le IP par defaut / 3e parametre optionnel
};
