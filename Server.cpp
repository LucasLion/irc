
#include "Server.hpp"
#include <sys/socket.h>

	
Server::Server( void ) : _maxClients(30) {
	_clientSockets.resize(_maxClients, 0);
}


static struct sockaddr_in initAddress(char *port) {

	struct sockaddr_in address;

	address.sin_family = AF_INET;
	address.sin_addr.s_addr = INADDR_ANY;
	address.sin_port = htons( atoi(port) );

	return (address);
}

int		Server::createSocket() {

	int opt = true;
	int master_socket;
		
	//create a master socket
	if( (master_socket = socket(AF_INET , SOCK_STREAM , 0)) == 0) {
		perror("socket failed");
		exit(EXIT_FAILURE);
	}
	
	std::cout << "master socket : " << master_socket << std::endl;
	//set master socket to allow multiple connections ,
	//this is just a good habit, it will work without this
	if( setsockopt(master_socket, SOL_SOCKET, SO_REUSEADDR, (char *)&opt, sizeof(opt)) < 0 ) {
		perror("setsockopt");
		exit(EXIT_FAILURE);
	}
	
	//bind the socket to localhost port 8888
	if (bind(master_socket, (struct sockaddr *)&_address, sizeof(_address)) < 0) {
		perror("bind failed");
		exit(EXIT_FAILURE);
	}
	std::cout << "Listener on port " <<  _portno << std::endl;
		
	//try to specify maximum of 3 pending connections for the master socket
	if (listen(master_socket, 3) < 0) {
		perror("listen");
		exit(EXIT_FAILURE);
	}
		
	//accept the incoming connection
	std::cout << "Waiting for connections ..." <<  _portno << std::endl;
	return (master_socket);	
}


Server::Server(char *port) : _maxClients(30)
{
	_clientSockets.resize(_maxClients, 0);
	_address = initAddress(port);
	_portno = atoi(port);
	_addrLen = sizeof(_address);
	_masterSocket = createSocket();
}

void	Server::setPort( char * port ) { _portno = atoi(port); }



/*int	Server::createSocket( int argc, char** argv ) {

	static_cast<void>(argc);
	int opt = true;
	int master_socket;
	//struct sockaddr_in address = initAddress(argv);
		
	//create a master socket
	if( (master_socket = socket(AF_INET , SOCK_STREAM , 0)) == 0) {
		perror("socket failed");
		exit(EXIT_FAILURE);
	}
	
	std::cout << "master socket : " << master_socket << std::endl;
	//set master socket to allow multiple connections ,
	//this is just a good habit, it will work without this
	if( setsockopt(master_socket, SOL_SOCKET, SO_REUSEADDR, (char *)&opt, sizeof(opt)) < 0 ) {
		perror("setsockopt");
		exit(EXIT_FAILURE);
	}
	
	//bind the socket to localhost port 8888
	if (bind(master_socket, (struct sockaddr *)&address, sizeof(address)) < 0) {
		perror("bind failed");
		exit(EXIT_FAILURE);
	}
	std::cout << "Listener on port " <<  _portno << std::endl;
		
	//try to specify maximum of 3 pending connections for the master socket
	if (listen(master_socket, 3) < 0) {
		perror("listen");
		exit(EXIT_FAILURE);
	}
		
	//accept the incoming connection
	puts("Waiting for connections ...");
	return (master_socket);
		
}
*/



/*void select_use(int master_socket )
{
	int max_sd = _mastersocket;
	int	sd;
	int	activity;
			
		//add child sockets to set
		for ( int i = 0 ; i < _maxClients ; i++) {
			//socket descriptor
			sd = _clientSockets[i];
				
			//if valid socket descriptor then add to read list
			if(sd > 0)
				FD_SET( sd , &readfds);
				
			//highest file descriptor number, need it for the select function
			if(sd > max_sd)
				max_sd = sd;
		}
	
		//wait for an activity on one of the sockets , timeout is NULL ,
		//so wait indefinitely
		activity = select( max_sd + 1 , &readfds , NULL , NULL , NULL);
	
		if ((activity < 0) && (errno!=EINTR))
			std::cout << "select error" << std::endl;

}*/


void Server::loop() {

	fd_set readfds;
	int max_sd, sd, new_socket, activity;
	std::string message = "Welcome to our first server !!!!!!!! \n YOU ARE CONNECTED \n";
	//struct sockaddr_in address = initAddress(argv);

	//int addrlen = sizeof(address);
	//int master_socket = createSocket(argc, argv);
	char buffer[1025];
	int valread;
	while (true) {
		//clear the socket set
		FD_ZERO(&readfds);
	
		//add master socket to set
		FD_SET(_masterSocket, &readfds);
		max_sd = _masterSocket;
			
		//add child sockets to set
		for ( int i = 0 ; i < _maxClients ; i++) {
			//socket descriptor
			sd = _clientSockets[i];
				
			//if valid socket descriptor then add to read list
			if(sd > 0)
				FD_SET( sd , &readfds);
				
			//highest file descriptor number, need it for the select function
			if(sd > max_sd)
				max_sd = sd;
		}
	
		//wait for an activity on one of the sockets , timeout is NULL ,
		//so wait indefinitely
		activity = select( max_sd + 1 , &readfds , NULL , NULL , NULL);
	
		if ((activity < 0) && (errno!=EINTR))
			std::cout << "select error" << std::endl;
			
		//If something happened on the master socket ,
		//then its an incoming connection
		if (FD_ISSET(_masterSocket, &readfds)) {
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
				
			puts("Welcome message sent successfully");
				
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
			
		//else its some IO operation on some other socket
		for (int i = 0; i < _maxClients; i++) {
			sd = _clientSockets[i];
				
			if (FD_ISSET( sd , &readfds)) {

				//incoming message
				bzero(buffer, 1025);
				valread = read(sd, buffer, 1024);
				std::cout << "\033[31m" << buffer << "\n\033[0m";
				if (strncmp(buffer, "CAP LS", 6) == 0)
					std::cout << "CAP LS TOUT EST PARÉ POUR LE DÉCOLLAGE" << std::endl;

				if (valread == 0) {
					//Somebody disconnected , get his details and print
					
					getpeername(sd , (struct sockaddr*)&_address , \
						(socklen_t*)&_addrLen);
					printf("Host disconnected , ip %s , port %d \n" ,
						inet_ntoa(_address.sin_addr) , ntohs(_address.sin_port));
						
					//Close the socket and mark as 0 in list for reuse
					close( sd );
					_clientSockets[i] = 0;
				}
					
				//Echo back the message that came in
				else {
					//set the string terminating NULL byte on the end
					//of the data read
					buffer[valread] = '\0';
					//send(sd , buffer , strlen(buffer) , 0 );
					send(sd , "message bien recu\n" , 19, 0 );
				}
			}
		}
	}
}
