
#include "../includes/header.hpp"
#include "../includes/Server.hpp"

void	Server::nickCmd( int sd, Message msg, User *user ) {
	(void)msg;
	(void)user;
	(void)sd;
	user->setNickName(msg.getParam(0));
	std::cout << "coucou\n";
	std::string test = ":welcome1!tonio@localhost NICK test\r\n" ; 
	send(sd, test.c_str(), test.length(),0);
	
	
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
	std::string protocol = "001 " + user->getNickName() + "Welcome to the Network, " + user->getNickName() + "\r\n";
	// possibilite d'ajouter le hostname etc
	send(sd, protocol.c_str(), protocol.length(), 0);
	
	//send(sd, ":localhost 001 utilisateur :Welcome to the , utilisateur\r\n", 71, 0);
	send(sd, ":localhost 002 :Welcome2 to FT_IRC\r\n", 35, 0 );


}

void	Server::passCmd( int sd, Message msg, User *user ) {
	(void)user;
	if (msg.getParam(0) != _passwd) {
		send(sd, ":localhost 464 utilisateur :Password incorrect\r\n", 51, 0 );
		throw std::exception();
	}
}
