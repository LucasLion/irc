
#include "../includes/header.hpp"
#include "../includes/Server.hpp"

void	Server::nickCmd( int sd, Message msg, User *user ) {
	(void)msg;
	(void)user;
	(void)sd;
}

void	Server::userCmd( int sd, Message msg, User *user ) {
	(void)msg;
	//msg.printCommand();

	if (!msg.getParam(0).length())
		send(sd, "localhost 461 :Not enough parameters\r\n", 53, 0 );
	if (user->getNickName().length())
		send(sd, "localhost 462 :You may not reregister\r\n", 53, 0 );
	else
		user->setNickName(msg.getParam(0));

	std::string protocol = ":localhost 001 utilisateur :Welcome to FT_IRC ";
	std::string name = msg.getParam(0).append("\r\n");
	protocol.append(name);
	send(sd, protocol.c_str(), protocol.length(), 0 );
	send(sd, ":localhost 002 :Welcome to FT_IRC\r\n", 35, 0 );
}

