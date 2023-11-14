
#include "../includes/header.hpp"
#include "../includes/Server.hpp"
#include "../includes/Channel.hpp"

#include <csignal>

int GSIGNALSTATUS = 0;

Server::Server( void ) : _nbClients(0) {}

Server::~Server( void ) {
	
	for (std::map<std::string, Channel*>::iterator it = _channels.begin(); it != _channels.end(); ++it) 
		delete it->second;
}


Server::Server( char *port, char *passwd ) : _nbClients(0) {
	char buffer[80];
	_creationTime = time(NULL);
	strftime(buffer, 40, "%a %b %d %H:%M:%S %Y", localtime(&_creationTime));
	_creationDate = buffer;
	_address.sin_family = AF_INET;
	_address.sin_addr.s_addr = INADDR_ANY;
	_address.sin_port = htons( atoi(port) );
	_portno = atoi(port);
	_addrLen = sizeof(_address);
	_masterSocket = createSocket();
	_max_sd = _masterSocket;
	_passwd = passwd;
	_name = "FT_IRC";
	_creationDate = std::time(NULL);
	_passOK = false;
	_numGuest= 1;
	_maxUsers = 0;
}


int		Server::createSocket( void ) {

	int opt = true;
	int masterSocket;
		
	if((masterSocket = socket(AF_INET , SOCK_STREAM , 0)) == 0) {
		perror("socket failed");
		exit(EXIT_FAILURE);
	}
	std::cout << "master socket : " << masterSocket << std::endl;
	if(setsockopt(masterSocket, SOL_SOCKET, SO_REUSEADDR, (char *)&opt, sizeof(opt)) < 0) {
		perror("setsockopt");
		exit(EXIT_FAILURE);
	}
	if (bind(masterSocket, (struct sockaddr *)&_address, sizeof(_address)) < 0) {
		perror("bind failed");
		exit(EXIT_FAILURE);
	}
	std::cout << "Listener on port " <<  _portno << std::endl;
	if (listen(masterSocket, 3) < 0) {
		perror("listen");
		exit(EXIT_FAILURE);
	}
	std::cout << "Waiting for connections ..." <<  _portno << std::endl;
	return (masterSocket);	
}

void Server::newConnection( void )
{
	int new_socket;

	if ((new_socket = accept(_masterSocket,
		(struct sockaddr *)&_address, (socklen_t*)&_addrLen))<0) {
			perror("accept");
			exit(EXIT_FAILURE);
	}
	std::cout << "New connection , socket fd is " << new_socket << ", ip is: " << inet_ntoa(_address.sin_addr) << ", port : " << ntohs(_address.sin_port) << std::endl;
	perror("send");
	_nbClients++;
	User new_user;
	new_user.setSd(new_socket);
	_users.push_back(new_user);
	_clientSockets.push_back(new_socket);
	std::cout << "Adding to list of sockets as " << _nbClients << std::endl;
}

void Server::handleConnections( void )
{
	int	sd;
	int	activity;
			

	FD_ZERO(&_readfds);
	FD_SET(_masterSocket, &_readfds);
	for (int i = 0; i < static_cast<int>(_clientSockets.size()); i++) {
		sd = _clientSockets[i];
		if(sd > 0)
			FD_SET(sd, &_readfds);
		if(sd > _max_sd)
			_max_sd = sd;
	}
	activity = select( _max_sd + 1, &_readfds, NULL, NULL, NULL);
	if ((activity < 0) && (errno!=EINTR))
		std::cout << "select error" << std::endl;
	if (FD_ISSET(_masterSocket, &_readfds)) {
		newConnection();
	}
}

std::string		ft_itoa(int n) {
	char num[10];
	sprintf(num, "%d", n);
	return (static_cast<std::string>(num));
}


void	sigint( int sig ) {
	(void)sig;
	throw std::runtime_error(ERROR("SERVER CLOSED PROPERLY ;)"));
}

void Server::run( void ) {

	int				valRead;
	//char			buffer[4608];
	
	signal(SIGINT, sigint);
	while (true) {
		handleConnections();
		for (size_t i = 0; i < _users.size(); i++) {
			if (FD_ISSET(_users[i].getSd(), &_readfds)) {
				char			buffer[1025] = {0};
				valRead = read(_users[i].getSd(), buffer, 1024);
				_users[i].getBuffer(buffer, valRead);

				if (valRead != 0) {
					if (generateResponse(&_users[i]) == false){
						close(_users[i].getSd());
                        _users.erase(_users.begin() + i);
						_clientSockets.erase(_clientSockets.begin() + i);
					}
				}
				else {
					getpeername(_users[i].getSd(), (struct sockaddr*)&_address, (socklen_t*)&_addrLen);
					std::cout << "Host disconnected, ip: " << inet_ntoa(_address.sin_addr) << " port: " << ntohs(_address.sin_port) << std::endl;
					disconnectChannels(_users[i].getNickName());
					close(_users[i].getSd());
					_users.erase(_users.begin() + i);
					_clientSockets.erase(_clientSockets.begin() + i);
				}
			}
		}
	}
}



bool	Server::createChannel( std::string channelName, std::string user ) {
	Channel* newChannel = new Channel; 
	newChannel->name = channelName;
	newChannel->operList.push_back(user);
	_channels[channelName] = newChannel;
	std::cout << SUCCESS("Channel \"" + channelName + "\" created") << std::endl;
	return (true);
}

bool Server::generateResponse( User *user ) {
	for (std::vector<Message>::iterator it = user->messages.begin(); it != user->messages.end();) {
		std::cout << "Command: " << it->rawMessage << std::endl;
		if(user->isRegistered() == false) {
			if (it->getCommand() == "NICK")
				nickPreRegistration(*it, user);
			if (it->getCommand() == "USER") 
				userCmd(*it, user);
			if (it->getCommand() == "PASS")
				passCmd(*it, user);
		}
		else {
			if (it->getCommand() == "USER")
				userCmd(*it, user);
			if (it->getCommand() == "NICK")
				nickCmd(*it, user);
			if (it->getCommand() == "PING")
				pongCmd(*it, user);
			if (it->getCommand() == "JOIN")
				joinCmd(*it, user);
			if (it->getCommand() == "TOPIC")
				topicCmd(*it, user);
			if (it->getCommand() == "PRIVMSG")
				prvMsgCmd(*it, user);
			if (it->getCommand() == "PONG")
				return (false);
			if (it->getCommand() == "KICK")
				kickCmd(*it, user);
			if (it->getCommand() == "INVITE")
				inviteCmd(*it, user);
			if (it->getCommand() == "WHO")
			 	whoCmd(*it, user);
			if (it->getCommand() == "PART")
			 	partCmd(*it, user);
			if (it->getCommand() == "MODE") 
					modeCmd(*it, user);
			if (it->getCommand() == "QUIT") {
				quitCmd(*it, user);
				return (false);
			}
		}
		if (it->getCommand() == "QUIT") {
			quitCmd(*it, user);
			return (false);
		}
		it = user->messages.erase(it);
	}
	return (true);
}

void	Server::connectServer( int sd, User *user) {

	std::string nbusers = ft_itoa(_users.size());
	if (_users.size() > _maxUsers)
		_maxUsers = _users.size();
	std::string maxuser = ft_itoa(_maxUsers);
	std::string nbchannels = ft_itoa(_channels.size());
	std::string nbservers = "0";
	std::string nbopers = "0";
	std::string nbinvisible = "0";
	std::string nbClients = "1";
	std::string modt =
"\n   _____ _____           ___ ____   ____\n \
 |  ___|_   _|         |_ _|  _ \\ / ___|\n \
 | |_    | |            | || |_) | |\n \
 |  _|   | |    _____   | ||  _ <| |___\n \
 |_|     |_|   |_____| |___|_| \\_\\\\____|\n\
       Welcome to the FT_IRC Server\n\
            42 school Project\n\
\n";

	sendClient(sd, RPL_WELCOME(user->getNickName()));
	sendClient(sd, RPL_YOURHOST(user->getNickName()));
	sendClient(sd, RPL_CREATED(user->getNickName(), currentDate()));
	sendClient(sd, RPL_MYINFO(user->getNickName()));
	sendClient(sd, RPL_ISUPPORT(user->getNickName()));
	sendClient(sd, RPL_ISUPPORT2(user->getNickName()));
	sendClient(sd, RPL_LUSERCLIENT(user->getNickName(), nbusers, nbinvisible, nbservers));
	sendClient(sd, RPL_LUSEROP(user->getNickName(), nbopers));
	sendClient(sd, RPL_LUSERCHANNELS(user->getNickName(), nbchannels));
	sendClient(sd, RPL_LUSERME(user->getNickName(), nbClients, nbservers));
	sendClient(sd, RPL_LOCALUSERS(user->getNickName(), nbusers, maxuser));
	sendClient(sd, RPL_MOTDSTART(user->getNickName()));
	sendClient(sd, RPL_MOTD(user->getNickName(),modt));
	sendClient(sd, RPL_MOTDEND(user->getNickName()));
}

int	Server::getPortno( void ) const { return _portno; }
std::map<std::string, Channel*>	*Server::getChannels( void ) { return &_channels; }
bool	Server::passOK() { return _passOK; }
void	Server::setPassOK(bool passOK) { _passOK = passOK;}
