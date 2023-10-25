
#include "../includes/header.hpp"
#include "../includes/Server.hpp"

bool is_valid(const std::string nickname){
	if (nickname.length() < 1 || nickname.length() > 32) {
        return false;
    }

    const std::string validChars = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789[]{}\\|";

    for (size_t i = 0; i < nickname.length(); ++i) {
        char c = nickname[i];
        if (i == 0 && !isalpha(c) && c != '[' && c != '{' && c != '\\' && c != '|') {
            return false;
        }
        if (i > 0 && validChars.find(c) == std::string::npos) {
            return false;
        }
    }
    return true;
}



void	Server::nickCmd( Message msg, User *user ) {
	
	std::string new_nick;
	std::string old_nick;
	std::string response;
	int sd = user->getSd();
	std::vector<User>::iterator it;
	std::map<std::string, Channel>::iterator it2;

	if (msg.getParam(0).length() == 0) {
				response = ":localhost 431 :No Nickname given \r\n";
				send(sd, response.c_str(), response.length(), 0);
				return;
			return;
	}

	new_nick = msg.getParam(0);
	for (it = _users.begin(); it != _users.end(); ++it) {
		if (new_nick == it->getNickName()) {
			response = ":localhost 433 " + new_nick + " :Nickname already in use \r\n";
			send(sd, response.c_str(), response.length(), 0);
			return;
		}
	}

	if (!is_valid(new_nick)) {
		response = ":localhost 432 " + new_nick + " :Erroneus Nickname \r\n";
		send(sd, response.c_str(), response.length(), 0);
		return;
	}

	if (user->isRegistered() == false) {
		
		user->setNickName(new_nick);
		user->setNickNameSet(true);
		if (user->isRealNameSet() && user->isNickNameSet() && _passOK )	{
			user->setRegistered(true);
			connectServer( sd, user);
		}
			
	}
	else {
		old_nick = user->getNickName();
		user->setNickName(new_nick);
		response = ":" + old_nick + "!" + old_nick + "@localhost NICK " + user->getNickName() + "\r\n";
		// TODO envoyer un message de conrimation a tous les channels ???

		for (it2 = _channels.begin(); it2 != _channels.end(); ++it2) {
			if (it2->second.isUserInChannel(old_nick)) {
				for (int i = 0; i < (int)it2->second.userList.size(); i++) {
					send(it2->second.userList[i]->getSd(), response.c_str(), response.length(), 0);
				}
			}
		}
		
		//for (int i = 0; i < (int)_channels[msg.getParam(0)].userList.size(); i++) {
		//	if (_channels[msg.getParam(0)].userList[i] != user->getNickName())
		//	send(_channels[msg.getParam(0)].userList[i].getSd(), response.c_str(), response.length(), 0);
		//}
		send(sd, response.c_str(), response.length(), 0);
	}
}

void	Server::userCmd( Message msg, User *user ) {
	
	int sd = user->getSd();
	std::string response ;
	if (!msg.getParam(0).length()){
		response = ":localhost 461 :Not enough parameters\r\n";
		send(sd, response.c_str(), response.length(), 0);
	}
	if (user->isRegistered())
		send(sd, ":localhost 462 :You may not reregister\r\n", 53, 0 );
	else {
		user->setUserName(msg.getParam(0));
		user->setRealName(msg.getParam(3));
	}
	user->setRegistered(true);
	connectServer(sd, user);
}

void	Server::pongCmd( Message msg, User *user ) {

	std::string response = ":localhost PONG localhost :" + msg.getParam(0) + "\r\n";
	send( user->getSd(), response.c_str(), response.length(), 0);
}

void	Server::passCmd( Message msg, User *user ) {
	
	if (msg.getParam(0) != _passwd) {
		std::string response =":localhost 464 " + user->getNickName() + " :Password incorrect\r\n"; 
		send(user->getSd(), response.c_str(), response.length(), 0 );
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
		user->addChannel(msg.getParam(0));
	}
	// check if the user is already in the channel
	if (_channels[msg.getParam(0)].isUserInChannel(user->getNickName())) {
		std::cout << "deja dans le channel: " << msg.getParam(0) << std::endl;
		response = ":localhost 403 " + user->getNickName() + " " + user->getNickName() + " :You are already in this channel\r\n";
		send(user->getSd(), response.c_str(), response.length(), 0);
	}
	else {
		_channels[msg.getParam(0)].addUser(user);
		user->addChannel(msg.getParam(0));
		response = ":" + user->getNickName() +  " JOIN " + msg.getParam(0) + "\r\n";
		// JOIN message
		send(user->getSd(), response.c_str(), response.length(), 0);
		// channel's topic
		response = ":localhost 332 " + user->getNickName() + " " + msg.getParam(0) + " :Default topic name (you can change it)\r\n";
		send(user->getSd(), response.c_str(), response.length(), 0);
		// create the list of users in the channels
		response = ":localhost 353 " + user->getNickName() + " = " + msg.getParam(0) + " :";
		for (int i = 0; i < (int)_channels[msg.getParam(0)].userList.size(); i++) {
			response += _channels[msg.getParam(0)].userList[i]->getNickName() + " ";
		}
		response += "\r\n";
		// send the list to everyone in the channel
		for (int i = 0; i < (int)_channels[msg.getParam(0)].userList.size(); i++) {
			std::cout << "1" << std::endl;
			// modifier ici, envoie a tout le monde dans le serveur au lieu de tout le monde dans le channel
			send(_users[i].getSd(), response.c_str(), response.length(), 0);
		}
		//for (int i = 0; i < (int)user->getChannels()[msg.getParam(0)]->userList.size(); i++) {
		//	std::cout << "2" << std::endl;
		//	send(_users[i].getSd(), response.c_str(), response.length(), 0);
		//}
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
		if (msg.getParam(0)[0] == '#') {
			for (int i = 0; i < (int)_channels[msg.getParam(0)].userList.size(); i++) {
				if (_channels[msg.getParam(0)].userList[i]->getNickName() != user->getNickName()) {
					response = ":" + user->getNickName() + " PRIVMSG " + msg.getParam(0) + " :" + msg.getParam(1) + "\r\n";
					send(_users[i].getSd(), response.c_str(), response.length(), 0);
				}
			}
		}
		// send private message to the user
		else if (msg.getParam(0)[0] != '#') {
			for (int i = 0; i < (int)_users.size(); i++) {
				if (_users[i].getNickName() == msg.getParam(0)) {
					response = ":" + user->getNickName() + " PRIVMSG " + msg.getParam(0) + " :" + msg.getParam(1) + "\r\n";
					send(_users[i].getSd(), response.c_str(), response.length(), 0);
				}
			}
		}
	}
}

void	Server::connectServer( int sd, User *user) {

	std::string response;
	
	char buffer[20];
	snprintf(buffer, 20, "%ld", _creationDate);
	std::string creationDate(buffer);

	response = ":localhost 001 " + user->getNickName() + " :Welcome1 to the " + _name + " Network, " + user->getNickName() + "\r\n";
	send(sd, response.c_str(), response.length(), 0);
	send(sd, response.c_str(), response.length(), 0);
	
	response = ":localhost 002 Welcome to FT_IRC\r\n";
	send(sd, response.c_str(), response.length(), 0);

	response = ":localhost 003 This server was created " + creationDate + "\r\n";
	send(sd, response.c_str(), response.length(), 0);

	response = ":localhost 004 " + _name + " localhost 1.0\r\n";
	send(sd, response.c_str(), response.length(), 0);
}