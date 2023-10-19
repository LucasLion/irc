
#include "../includes/header.hpp"
#include "../includes/Server.hpp"
#include "../includes/Channel.hpp"

#include <csignal>

int GSIGNALSTATUS = 0;

Server::Server( void ) : _nbClients(0) {
	//_clientSockets.resize(nbClients, 0);
}

Server::Server( char *port, char *passwd ) : _nbClients(0)
{
	//_clientSockets.resize(nbClients, 0);
	_address.sin_family = AF_INET;
	_address.sin_addr.s_addr = INADDR_ANY;
	_address.sin_port = htons( atoi(port) );
	_portno = atoi(port);
	_addrLen = sizeof(_address);
	_masterSocket = createSocket();
	_max_sd = _masterSocket;
	_passwd = passwd;
	_name = "FT_IRC";
	// rajouter le IP par defaut / 3e parametre optionnel
}

int		Server::createSocket( void ) {

	int opt = true;
	int masterSocket;
		
	//create a master socket
	if((masterSocket = socket(AF_INET , SOCK_STREAM , 0)) == 0) {
		perror("socket failed");
		exit(EXIT_FAILURE);
	}
	std::cout << "master socket : " << masterSocket << std::endl;
	//set master socket to allow multiple connections ,
	//this is just a good habit, it will work without this
	if(setsockopt(masterSocket, SOL_SOCKET, SO_REUSEADDR, (char *)&opt, sizeof(opt)) < 0) {
		perror("setsockopt");
		exit(EXIT_FAILURE);
	}
	//bind the socket to localhost port 8888
	if (bind(masterSocket, (struct sockaddr *)&_address, sizeof(_address)) < 0) {
		perror("bind failed");
		exit(EXIT_FAILURE);
	}
	std::cout << "Listener on port " <<  _portno << std::endl;
	//try to specify maximum of 3 pending connections for the master socket
	if (listen(masterSocket, 3) < 0) {
		perror("listen");
		exit(EXIT_FAILURE);
	}
	//accept the incoming connection
	std::cout << "Waiting for connections ..." <<  _portno << std::endl;
	return (masterSocket);	
}

void Server::newConnection( void )
{
	int new_socket;
	//std::string message = "Welcome to FT_IRC server\r\n";

	if ((new_socket = accept(_masterSocket,
		(struct sockaddr *)&_address, (socklen_t*)&_addrLen))<0) {
			perror("accept");
			exit(EXIT_FAILURE);
	}
	//inform user of socket number - used in send and receive commands
	std::cout << "New connection , socket fd is " << new_socket << ", ip is: " << inet_ntoa(_address.sin_addr) << ", port : " << ntohs(_address.sin_port) << std::endl;
		
	//send new connection greeting message
	//if(send(new_socket, message.c_str(), message.length(), 0) != (ssize_t)message.length()) {
		perror("send");
	//}
	//add new socket to array of sockets
	//for (int i = 0; i < nbClients; i++) {
		//if position is empty
		//if( _clientSockets[i] == 0 ) {
			//_clientSockets[i] = new_socket;
			_nbClients++;
			User new_user;
			new_user.setSd(new_socket);
			_users.push_back(new_user);
			_clientSockets.push_back(new_socket);
			std::cout << "Adding to list of sockets as " << _nbClients << std::endl;
			//break;
		//}
	//}
}

void Server::handleConnections( void )
{
	int	sd;
	int	activity;
			
	//clear socket set and add mastersocket
	FD_ZERO(&_readfds);
	FD_SET(_masterSocket, &_readfds);
	//add child sockets to set
	for (int i = 0; i < static_cast<int>(_clientSockets.size()); i++) {
		sd = _clientSockets[i];
	//if valid socket descriptor then add to read list
		if(sd > 0)
			FD_SET(sd, &_readfds);
	//highest file descriptor number, need it for the select function
		if(sd > _max_sd)
			_max_sd = sd;
	}
	//wait for an activity on one of the sockets , timeout is NULL ,so wait indefinitely
	activity = select( _max_sd + 1, &_readfds, NULL, NULL, NULL);
	if ((activity < 0) && (errno!=EINTR))
		std::cout << "select error" << std::endl;
	//If something happened on the master socket ,
	//then its an incoming connection
	if (FD_ISSET(_masterSocket, &_readfds)) {
		newConnection();
	}
}

//void	handleSignal2(int signal) {
//	if (signal == SIGINT) {
//		gSignalStatus2 = 1;
//		std::cout << "signal received in handleSignal2: " << 1 << std::endl; }
//		return ;
//	}

void Server::run( void ) {

	int				valRead;
	char			buffer[4608];
	
	while (true) {
		handleConnections();

		//Ajout de nouveaux utilisateurs
		//  while (_users.size() < _clientSockets.size()) {
          //  User new_user;
        //    _users.push_back(new_user);
            //_users.back().setSd(_clientSockets[_users.size() - 1]);
        //}

		//else its some IO operation on some other socket
		for (int i = 0; i < static_cast<int>(_users.size()); i++) {
			if (FD_ISSET(_users[i].getSd(), &_readfds)) {
				//incoming message
				bzero(buffer, 1025);
				valRead = read(_users[i].getSd(), buffer, 1024);
				_users[i].getBuffer(buffer);
				if (valRead != 0) {
					if (generateResponse(&_users[i]) == false){
						close(_users[i].getSd());
                        _users.erase(_users.begin() + i);
						return;
					}
				}
				else {
					//Somebody disconnected , get his details and print
					getpeername(_users[i].getSd() , (struct sockaddr*)&_address, (socklen_t*)&_addrLen);
					std::cout << "Host disconnected, ip: " << inet_ntoa(_address.sin_addr) << " port: " << ntohs(_address.sin_port) << std::endl;
					//Close the socket and mark as 0 in list for reuse
					close(_users[i].getSd());
					_users.erase(_users.begin() + i);
					//_clientSockets[i] = 0;
				}
			}
		}
	}
}

void	Server::getBuffer( char *buf ) {

	size_t	                start = 0;
    size_t	                crlfPos;

    _buffer.assign(buf, strlen(buf));

	std::cout << "buffer : " << _buffer << std::endl;
    while ((crlfPos = _buffer.find("\r\n", start)) != std::string::npos) {
        Message msg;
        msg.rawMessage = (_buffer.substr(start, crlfPos - start));
		msg.parseInput();
        _messages.push_back(msg); 
        start = crlfPos + 2;
    }
}

bool	Server::createChannel( std::string channelName ) {

	try {
		if (_channels.find(channelName) != _channels.end())
			throw Channel::ChannelAlreadyExistsException();
	} catch	(std::exception &e) {
		std::cout << e.what() << std::endl;
		return false;
	}

	Channel newChannel;
	newChannel.name = channelName;
	_channels[channelName] = newChannel;
	std::cout << SUCCESS("Channel \"" + channelName + "\" created") << std::endl;
	for (std::map<std::string, Channel>::iterator it = _channels.begin(); it != _channels.end(); it++) {
		std::cout << "Channel: " << it->first << std::endl;
		// iterate through the vector of users in the channel 
		for (int i = 0; i < (int)it->second.userList.size(); i++) {
			std::cout << "User: " << it->second.userList[i] << std::endl;
		}
	}
	return true;
}

bool Server::generateResponse( User *user ) {
	for (std::vector<Message>::iterator it = user->messages.begin(); it != user->messages.end();) {
		std::cout << "COMMAND_RECEIVED: " << it->rawMessage << std::endl;
		if (it->getCommand() == "CAP") {
			send( user->getSd(), "CAP * LS\r\n", 12, 0 );
		}
		if (it->getCommand() == "NICK") {
			nickCmd(*it, user);
		}
		if (it->getCommand() == "USER") {
			userCmd(*it, user);
		}
		if (it->getCommand() == "PASS") {
			passCmd(*it, user);
		}
		if (it->getCommand() == "PING") {
			std::cout << SUCCESS("PING PONG") << std::endl;
			pongCmd(*it, user);
		}
		if (it->getCommand() == "PONG") {
			// for TESTS
			return (false);
		}
		if (it->getCommand() == "WHOIS") {
			send(user->getSd(), ":localhost 318 THE_BEST_NICKNAME :End of /WHOIS list", 51, 0);
		}
		if (it->getCommand() == "JOIN") {
			joinCmd(*it, user);
		}
		it = user->messages.erase(it);
    }
	return (true);
}

int	Server::getPortno( void ) const { return _portno; }

std::map<std::string, Channel>	Server::getChannels( void ) const { return _channels; }
