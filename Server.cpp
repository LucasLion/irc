
#include "Server.hpp"

Server::Server( void ) : _maxClients(30) {
	_clientSockets.resize(_maxClients, 0);
}

Server::Server( char *port, char *passwd ) : _maxClients(30)
{
	_clientSockets.resize(_maxClients, 0);
	_address.sin_family = AF_INET;
	_address.sin_addr.s_addr = INADDR_ANY;
	_address.sin_port = htons( atoi(port) );
	_portno = atoi(port);
	_addrLen = sizeof(_address);
	_masterSocket = createSocket();
	_max_sd = _masterSocket;
	_passwd = passwd;
	loop();
}

int		Server::createSocket( void ) {

	int opt = true;
	int masterSocket;
		
	//create a master socket
	if( (masterSocket = socket(AF_INET , SOCK_STREAM , 0)) == 0) {
		perror("socket failed");
		exit(EXIT_FAILURE);
	}
	
	std::cout << "master socket : " << masterSocket << std::endl;
	//set master socket to allow multiple connections ,
	//this is just a good habit, it will work without this
	if( setsockopt(masterSocket, SOL_SOCKET, SO_REUSEADDR, (char *)&opt, sizeof(opt)) < 0 ) {
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
	std::string message = "Welcome to our first server !!!!!!!! \n YOU ARE CONNECTED \n";

	if ((new_socket = accept(_masterSocket,
		(struct sockaddr *)&_address, (socklen_t*)&_addrLen))<0) {
			perror("accept");
			exit(EXIT_FAILURE);
	}
			
	//inform user of socket number - used in send and receive commands
	std::cout << "New connection , socket fd is " << new_socket << ", ip is: " << inet_ntoa(_address.sin_addr) << ", port : " << ntohs(_address.sin_port) << std::endl;
		
	//send new connection greeting message
	if(send(new_socket, message.c_str(), message.length(), 0) != (ssize_t)message.length()) {
		perror("send");
	}
	std::cout << "Welcome message sent successfully" << std::endl;
	
	//add new socket to array of sockets
	for (int i = 0; i < _maxClients; i++) {
		//if position is empty
		if( _clientSockets[i] == 0 ) {
			_clientSockets[i] = new_socket;
			std::cout << "Adding to list of sockets as " << i << std::endl;
			break;
		}
	}

}


void Server::handleConnections( void )
{
	int	sd;
	int	activity;
			
	//clear socket set and add mastersocket
	FD_ZERO(&_readfds);
	FD_SET(_masterSocket, &_readfds);
		
	//add child sockets to set
	for (int i = 0; i < _maxClients; i++) {
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

void Server::loop( void ) {

	int				sd;
	int				valRead;
	char			buffer[4608];
	
	while (true) {
		handleConnections();
			
		//else its some IO operation on some other socket
		for (int i = 0; i < _maxClients; i++) {
			sd = _clientSockets[i];
				
			if (FD_ISSET(sd, &_readfds)) {

				//incoming message
				bzero(buffer, 1025);
				valRead = read(sd, buffer, 1024);
				_users[i].addBuffer(buffer);
				_users[i].parseBuffer();
				_users[i].printCommands();
				//std::string datareceived(buffer);
				//std::cout << "\033[31m" << datareceived << "\n\033[0m";



				if (valRead == 0) {
					//Somebody disconnected , get his details and print
					
					getpeername(sd , (struct sockaddr*)&_address, (socklen_t*)&_addrLen);
					std::cout << "Host disconnected, ip: " << inet_ntoa(_address.sin_addr) << " port: " << ntohs(_address.sin_port) << std::endl;
						
					//Close the socket and mark as 0 in list for reuse
					close(sd);
					_clientSockets[i] = 0;
				}
					
				//Echo back the message that came in
				else {
					//set the string terminating NULL byte on the end of the data read
					_buffer[valRead] = '\0';
					//send(sd, "message bien recu\n", 19, 0 );
				}
			}
		}
	}
}


