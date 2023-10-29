
#pragma once

#include "header.hpp"
#include "Channel.hpp"
#include "User.hpp"

class Server {

public:

						Server( void );
						Server( char *port, char *passwd );
	std::string			getLocalIp( void );
	int					createSocket( void );
	void				handleConnections( void );
	void				newConnection( void );
    void                getBuffer( char * buf );
	void				run( void );
	bool				createChannel( std::string name );
	void				sendError(std::string code_Error, int sd);
	bool				generateResponse( User *user );
	void				userCmd( Message msg, User *user );
	void				nickCmd( Message msg, User *user );
	void				nickPreRegistration( Message msg, User *user );
	void				passCmd( Message msg, User *user );
	void				joinCmd( Message msg, User *user );
	void				pongCmd( Message msg, User *user );
	void				topicCmd( Message msg, User *user );
	void				prvMsgCmd( Message msg, User *user );
	int					getPortno( void ) const;
	bool				passOK();
	void				setPassOK(bool passOK);
	void				connectServer( int sd , User *user );
	void				sendClient(int sd, std::string response);


	std::map<std::string, Channel*>	getChannels( void ) ;

private:
	
	std::string						_name;
	int								_nbClients;
	int								_portno;
	std::vector<int>				_clientSockets;
	struct sockaddr_in				_address;
	int								_addrLen;
	int								_masterSocket;
	fd_set							_readfds;
	std::string						_buffer;
	int								_max_sd;
	std::string						_passwd;
	std::map<std::string, Channel*>	_channels;
	std::vector<User>				_users;
    std::vector<Message>		    _messages;
	bool							_passOK;
	std::time_t						_creationDate;
	int								_numGuest;
	// rajouter le IP par defaut / 3e parametre optionnel
};
