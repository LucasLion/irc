
#include "../includes/header.hpp"
#include "../includes/Server.hpp"

void	Server::nickCmd( int sd, Message msg, User *user ) {
	(void)msg;
	(void)user;
	(void)sd;
	user->setNickName(msg.getParam(0));

	// faire un check si le nickname est deja pris
	// faire un check si le nickname est valide
	// envoyer un message de confirmation
}

void	Server::userCmd( int sd, Message msg, User *user ) {
	(void)msg;

	msg.printCommand();
	if (!msg.getParam(0).length())
		send(sd, "localhost 461 :Not enough parameters\r\n", 53, 0 );
	//if (user->getNickName().length())
	//	send(sd, "localhost 462 :You may not reregister\r\n", 53, 0 );
	else {
	//	user->setNickName(msg.getParam(0));
		user->setRealName(msg.getParam(3));
	}
	// verifier que le nickname est bien set 
	// verifier que le username est bien set	
	// verifier que le realname est bien set	
	// verifier que le hostname est bien set
	// envoyer un message de confirmation
	//"<client> :Welcome to the <networkname> Network, <nick>[!<user>@<host>]"
	std::string protocol = ":localhost 001 " +  user->getNickName() + " :Welcome to the " + _name + " Network, " + user->getNickName() + "\r\n";
	// possibilite d'ajouter le hostname etc
	send(sd, protocol.c_str(), protocol.length(), 0);


}

void	Server::pongCmd( int sd, Message msg, User *user ) {
	(void)user;
	std::string response = ":localhost PONG localhost :" + msg.getParam(0) + "\r\n";
	send(sd, response.c_str(), response.length(), 0);
}

void	Server::passCmd( int sd, Message msg, User *user ) {
	(void)user;
	if (msg.getParam(0) != _passwd) {
		send(sd, ":localhost 464 utilisateur :Password incorrect\r\n", 51, 0 );
		throw std::exception();
	}
}

void	Server::joinCmd( int sd, Message msg, User *user ) {
	(void)user;

	// TODO parse command for multiple channels at once
	// TODO check later for all the other errors
	
	if (msg.getParam(0).length() == 0) {
		send(sd, ":localhost 461 utilisateur JOIN :Not enough parameters\r\n", 59, 0 );
		return ;
	}
	// check if the channel name is valide
	if (msg.getParam(0)[0] != '#') {
		send(sd, ":localhost 403 utilisateur :No such channel\r\n", 47, 0 );
		return ;
	}
	// check if the channel exists and create it if not
	if (_channels.find(msg.getParam(0)) == _channels.end()) {
		this->createChannel(msg.getParam(0));
	}
	// check if the user is already in the channel
	if (_channels[msg.getParam(0)].isUserInChannel(user->getNickName())) {
		std::cout << "deja dans le channel: " << msg.getParam(0) << std::endl;
		send(sd, ":localhost 403 utilisateur :You are already in this channel\r\n", 61, 0 );
	}
	else {
		_channels[msg.getParam(0)].addUser(user->getNickName());
		send(sd, ":localhost 332 utilisateur :Bienvenue sur le channel\r\n", 55, 0 );
	}
}

