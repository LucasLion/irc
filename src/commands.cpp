
#include "../includes/header.hpp"
#include "../includes/Server.hpp"


void	Server::nickCmd( Message msg, User *user ) {
	
	int sd = user->getSd();
	msg.printCommand();
	std::string old_nick = user->getNickName();
	std::string new_nick = msg.getParam(0);

	
	user->setNickName(msg.getParam(0));

	std::string protocol = ":" + old_nick + "!" + old_nick + "@localhost NICK " + user->getNickName() + "\r\n";
	// possibilite d'ajouter le hostname etc
	send(sd, protocol.c_str(), protocol.length(), 0);
	

	// faire un check si le nickname est deja pris
	// faire un check si le nickname est valide
	// envoyer un message de confirmation
}

void	Server::userCmd( Message msg, User *user ) {
	
	int sd = user->getSd();
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
	std::string response = ":localhost 001 " + user->getNickName() + " Welcome1 to the " + _name + " Network, " + user->getNickName() + "\r\n";
	// possibilite d'ajouter le hostname etc
	send(sd, response.c_str(), response.length(), 0);
	send(sd, response.c_str(), response.length(), 0);
	//send(sd, ":localhost 001 test Welcome1 to server \r\n", 60, 0);
	
	//send(sd, ":localhost 001 utilisateur :Welcome to the , utilisateur\r\n", 71, 0);
	send(sd, ":localhost 002 Welcome2 to FT_IRC\r\n", 35, 0 );

}

void	Server::pongCmd( Message msg, User *user ) {

	std::string response = ":localhost PONG localhost :" + msg.getParam(0) + "\r\n";
	send( user->getSd(), response.c_str(), response.length(), 0);
}

void	Server::passCmd( Message msg, User *user ) {
	
	if (msg.getParam(0) != _passwd) {
		send(user->getSd(), ":localhost 464 utilisateur :Password incorrect\r\n", 51, 0 );
		throw std::exception();
	}
}

void	Server::joinCmd( Message msg, User *user ) {
	

	// TODO parse command for multiple channels at once
	// TODO check later for all the other errors
	
	std::string response = "";

	if (msg.getParam(0).length() == 0) {
		response = ":localhost 461 " + user->getNickName() + " JOIN :Not enough parameters\r\n";
		send(user->getSd(), response.c_str(), response.length(), 0);
		return ;
	}
	// check if the channel name is valide
	if (msg.getParam(0)[0] != '#') {
		response = ":localhost 403 " + user->getNickName() + " " + msg.getParam(0) + " :No such channel\r\n";
		send(user->getSd(), response.c_str(), response.length(), 0);
		return ;
	}
	// check if the channel exists and create it if not
	if (_channels.find(msg.getParam(0)) == _channels.end()) {
		this->createChannel(msg.getParam(0));
	}
	// check if the user is already in the channel
	if (_channels[msg.getParam(0)].isUserInChannel(user->getNickName())) {
		std::cout << "deja dans le channel: " << msg.getParam(0) << std::endl;
		response = ":localhost 403 " + user->getNickName() + " utilisateur :You are already in this channel\r\n";
		send(user->getSd(), response.c_str(), response.length(), 0);
	}
	else {
		_channels[msg.getParam(0)].addUser(user->getNickName());
		response = ":" + user->getNickName() +  " JOIN " + msg.getParam(0) + "\r\n";
		// JOIN message
		send(user->getSd(), response.c_str(), response.length(), 0);
		// channel's topic
		response = ":localhost 332 " + user->getNickName() + " " + msg.getParam(0) + " :Default topic name (you can change it)\r\n";
		send(user->getSd(), response.c_str(), response.length(), 0);
		// create the list of users in the channels
		response = ":localhost 353 " + user->getNickName() + " = " + msg.getParam(0) + " :";
		for (int i = 0; i < (int)_channels[msg.getParam(0)].userList.size(); i++) {
			response += _channels[msg.getParam(0)].userList[i] + " ";
		}
		response += "\r\n";
		// send the list to everyone in the channel
		for (int i = 0; i < (int)_channels[msg.getParam(0)].userList.size(); i++) {
			send(_users[i].getSd(), response.c_str(), response.length(), 0);
		}
	}
}

void	Server::topicCmd( Message msg, User *user ) {
	std::string response = "";

	std::cout << "param: " << msg.getParam(0) << std::endl;
	if (msg.getParam(0).length() == 0) {
		response = ":localhost 461 " + user->getNickName() + " TOPIC :Not enough parameters\r\n";
		send(user->getSd(), response.c_str(), response.length(), 0);
		return ;
	}
	if (_channels.find(msg.getParam(0)) == _channels.end()) {
		response = ":localhost 442 " + user->getNickName() + " " + msg.getParam(0) + " :You're not on that channel\r\n";
		send(user->getSd(), response.c_str(), response.length(), 0);
		return ;
	}
	else {
		// Here change the topic of the channel
		// check if user has rights
		std::string newName = "newName";
		response = ":localhost 332 " + user->getNickName() + " " + msg.getParam(0) + " :" + newName + "\r\n";
		for (int i = 0; i < (int)_channels[msg.getParam(0)].userList.size(); i++) {
			send(_users[i].getSd(), response.c_str(), response.length(), 0);
		}
	}
}

void	Server::prvMsgCmd( Message msg, User *user ) {
	std::string response = "";
	if (msg.getParam(0).length() == 0) {
		response = ":localhost 411 " + user->getNickName() + " :No recipient given (PRIVMSG)\r\n";
		send(user->getSd(), response.c_str(), response.length(), 0);
		return ;
	}
	if (msg.getParam(1).length() == 0) {
		response = ":localhost 412 " + user->getNickName() + " :No text to send\r\n";
		send(user->getSd(), response.c_str(), response.length(), 0);
		return ;
	}
	if (user->getChannels().find(msg.getParam(0)) == user->getChannels().end()) {
		response = ":localhost 401 " + user->getNickName() + " " + msg.getParam(0) + " :No such nick/channel\r\n";
		send(user->getSd(), response.c_str(), response.length(), 0);
		return ;
	}
	else {
		// send the message to all the users in the channel
		//for (int i = 0; i < (int)_channels[msg.getParam(0)].userList.size(); i++) {
		for (int i = 0; i < (int)_channels[msg.getParam(0)].userList.size(); i++) {
			if (_channels[msg.getParam(0)].userList[i] != user->getNickName()) {
				response = ":" + user->getNickName() + " PRIVMSG " + msg.getParam(0) + " :" + msg.getParam(1) + "\r\n";
				send(_users[i].getSd(), response.c_str(), response.length(), 0);
			}
		}
	}
}
