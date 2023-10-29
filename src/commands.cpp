
#include "../includes/header.hpp"
#include "../includes/Server.hpp"

bool is_valid(const std::string nickname) {
	if (nickname.length() < 1 || nickname.length() > 32)
        return false;
    const std::string validChars = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789[]{}\\|";
    for (size_t i = 0; i < nickname.length(); ++i) {
        char c = nickname[i];
        if (i == 0 && !isalpha(c) && c != '[' && c != '{' && c != '\\' && c != '|')
            return false;
        if (i > 0 && validChars.find(c) == std::string::npos)
            return false;
    }
    return true;
}

void	Server::sendClient(int sd, std::string response) {
	///send(sd, response.c_str(), response.length(), 0);
	write(sd, response.c_str(), response.length());
}





void	Server::nickPreRegistration( Message msg, User *user ) {
	
	std::string new_nick;
	std::string old_nick;
	int sd = user->getSd();
	char num[10];
	std::string nu;
	std::string response;
	std::vector<User>::iterator it;
	std::map<std::string, Channel*>::iterator it2;

	sprintf(num, "%d", _numGuest);

	if (msg.getParam(0).length() == 0) {
		_numGuest++;
		nu = static_cast<std::string>(num);
		new_nick = "guest" + nu;
		response = "we gave you the nickname :  " + new_nick;
		//write(sd, response.c_str(), response.length());
		//sendClient(sd, response);
	}

	new_nick = msg.getParam(0);
	for (it = _users.begin(); it != _users.end(); ++it) {
		if (new_nick == it->getNickName()) {
			_numGuest++;
			nu = static_cast<std::string>(num);
			new_nick = "guest" + nu;
			response = "we gave you the nickname :  " + new_nick;
			//write(sd, response.c_str(), response.length());
			//sendClient(sd, response);
		}
	}

	if (!is_valid(new_nick)) {
			_numGuest++;
			nu = static_cast<std::string>(num);
			new_nick = "guest" + nu;
			response = "we gave you the nickname :  " + new_nick;
			//write(sd, response.c_str(), response.length());
			//sendClient(sd, response);
	}

	user->setNickName(new_nick);
	user->setNickNameSet(true);
		if(user->isPassOK() && user->isRealNameSet()){
			user->setRegistered(true);
			std::cout << "user registered dans NICK" << std::endl;
			connectServer(sd, user);
		}

}

void	Server::nickCmd( Message msg, User *user ) {
	
	std::string new_nick;
	std::string old_nick;
	int sd = user->getSd();
	std::vector<User>::iterator it;
	std::map<std::string, Channel*>::iterator it2;

	std::cout << "nickCmd" << std::endl;
	std::cout << "msg : " << msg.getParam(0) << std::endl;

	if (msg.getParam(0).length() == 0) {
		sendClient(sd, ERR_NONICKNAMEGIVEN(user->getNickName()));
		return;
	}

	new_nick = msg.getParam(0);
	for (it = _users.begin(); it != _users.end(); ++it) {
		if (new_nick == it->getNickName()) {
			sendClient(sd, ERR_NICKNAMEINUSE(user->getNickName(), new_nick));
			return;
		}
	}

	if (!is_valid(new_nick)) {
		sendClient(sd, ERR_ERRONEUSNICKNAME(user->getNickName(), new_nick));
		return;
	}
	
	old_nick = user->getNickName();
	user->setNickName(new_nick);
	if (user->_channels.size() > 0){
		for (it2 = user->_channels.begin(); it2 != user->_channels.end(); ++it2) {
			for (int i = 0 ; i < (int)it2->second->userList.size(); i++) {
				std::cout << it2->second->userList[i]->getSd() << std::endl;
				sendClient(it2->second->userList[i]->getSd(), NICK(old_nick, new_nick));
			}
		}
	}else{
		std::cout << "new nick : " << new_nick << std::endl;
		old_nick += "!" + user->getUserName() + "@localhost";
		std::cout << "old nick : " << old_nick << std::endl;
		std::string response = NICK(old_nick, new_nick);
		std::cout << "response : " << response << std::endl;
		sendClient(sd, NICK(old_nick, new_nick));
	}
}

void	Server::userCmd( Message msg, User *user ) {
	
	if (!msg.getParam(0).length()){
		sendClient(user->getSd(), ERR_NEEDMOREPARAMS(user->getNickName(), msg.getCommand()));
	}
	if (user->isRegistered())
		sendClient(user->getSd(), ERR_ALREADYREGISTERED(user->getNickName()));
	if(user->isRealNameSet())
		sendClient(user->getSd(), ERR_ALREADYREGISTERED(user->getNickName()));
	if (user->isPassOK() == false)
		sendClient(user->getSd(), ERR_PASSWDMISMATCH(user->getNickName()));
	else {
		user->setUserName(msg.getParam(0));
		user->setRealName(msg.getParam(3));
		user->setRealNameSet(true);
	}
	if (user->isNickNameSet() && user->isPassOK())	{
			user->setRegistered(true);
			std::cout << "user registered dans USER" << std::endl;
			connectServer(user->getSd(), user);
		}		
}



void	Server::passCmd( Message msg, User *user ) {

	if (msg.getParam(0).length() > 0) {
		user->setPass(msg.getParam(0));
		if(user->getPass() == _passwd)
			user->setPassOK(true);
		else
			user->setPassOK(false);
	}
		//throw std::exception();
		//sendClient(user->getSd(), ERR_PASSWDMISMATCH(user->getNickName()));
		
	
}



void	Server::pongCmd( Message msg, User *user ) {
	sendClient(user->getSd(), PONG(msg.getParam(0)));
}

void	Server::joinCmd( Message msg, User *user ) {
	
	if (msg.getParam(0).length() == 0) {
		sendClient(user->getSd(), ERR_NEEDMOREPARAMS(user->getNickName(), msg.getCommand()));
		return ;
	}
	// check if the channel name is valid
	if (msg.getParam(0)[0] != '#') {
		sendClient(user->getSd(), ERR_NOSUCHCHANNEL(user->getNickName(), msg.getParam(0)));
		return ;
	}
	// check if the channel exists and create it if not
	if (_channels.find(msg.getParam(0)) == _channels.end()) {
		this->createChannel(msg.getParam(0));
		//user->addChannel(msg.getParam(0), , &(_channels[msg.getParam(0)])));
	}
	// check if the user is already in the channel
	if (_channels[msg.getParam(0)]->isUserInChannel(user->getNickName())) {
		sendClient(user->getSd(), ERR_USERONCHANNEL(user->getNickName(), user->getNickName(), msg.getParam(0)));
	}
	else {
		_channels[msg.getParam(0)]->addUser(user);
		user->addChannel(msg.getParam(0), _channels[msg.getParam(0)]);
		sendClient(user->getSd(), JOIN(user->getNickName(), msg.getParam(0)));
		// channel's topic
		sendClient(user->getSd(), RPL_TOPIC(user->getNickName(), msg.getParam(0), "default topic (has to change)"));

		// create the list of users in the channels
		std::string response = ":localhost 353 " + user->getNickName() + " = " + msg.getParam(0) + " :";

		for (int i = 0; i < (int)_channels[msg.getParam(0)]->userList.size(); i++) {
			response += _channels[msg.getParam(0)]->userList[i]->getNickName() + " ";
		}
		response += "\r\n";
//		 send the list to everyone in the channel
		for (int i = 0; i < (int)_channels[msg.getParam(0)]->userList.size(); i++) {
			// modifier ici, envoie a tout le monde dans le serveur au lieu de tout le monde dans le channel
			send(_users[i].getSd(), response.c_str(), response.length(), 0);
			//sendClient(user->getSd(), ERR_PASSWDMISMATCH(user->getNickName()));
		}
		
		// loop with iterator to send the list to everyone in the channels
		std::vector<User*>::iterator it;
		for (it = _channels[msg.getParam(0)]->userList.begin(); it != _channels[msg.getParam(0)]->userList.end(); ++it) {
			sendClient((*it)->getSd(), RPL_NAMREPLY(user->getNickName(), "=", msg.getParam(0), "", (*it)->getNickName()));
		};	


		for (int i = 0; i < (int)user->getChannels()[msg.getParam(0)]->userList.size(); i++) {
			std::cout << "2" << std::endl;
			send(_users[i].getSd(), response.c_str(), response.length(), 0);
		}
	}
	}

void	Server::topicCmd( Message msg, User *user ) {
	std::string response = "";

	std::cout << "param: " << msg.getParam(0) << std::endl;
	if (msg.getParam(0).length() == 0) {
		sendClient(user->getSd(), ERR_NEEDMOREPARAMS(user->getNickName(), msg.getCommand()));
		return ;
	}
	if (_channels.find(msg.getParam(0)) == _channels.end()) {
		sendClient(user->getSd(), ERR_NOTONCHANNEL(user->getNickName(), msg.getParam(0)));
		return ;
	}
	else {
		// Here change the topic of the channel
		// check if user has rights
		std::string newName = "newName";
		for (int i = 0; i < (int)_channels[msg.getParam(0)]->userList.size(); i++) {
			sendClient(user->getSd(), RPL_TOPIC(user->getNickName(), msg.getParam(0), newName));
		}
	}
}

void	Server::prvMsgCmd( Message msg, User *user ) {
	if (msg.getParam(0).length() == 0) {
		sendClient(user->getSd(), ERR_NORECIPIENT(user->getNickName()));
		return ;
	}
	if (msg.getParam(1).length() == 0) {
		sendClient(user->getSd(), ERR_NOTEXTTOSEND(user->getNickName()));
		return ;
	}
	if (user->getChannels().find(msg.getParam(0)) == user->getChannels().end()) {
		sendClient(user->getSd(), ERR_NOSUCHNICK(user->getNickName(), msg.getParam(0)));
		return ;
	}
	else {
		// send the message to all the users in the channel
		if (msg.getParam(0)[0] == '#') {
			for (int i = 0; i < (int)_channels[msg.getParam(0)]->userList.size(); i++) {
				if (_channels[msg.getParam(0)]->userList[i]->getNickName() != user->getNickName()) {
					sendClient(_channels[msg.getParam(0)]->userList[i]->getSd(), PRIVMSG(user->getNickName(), msg.getParam(0), msg.getParam(1)));
				}
			}
		}
		// send private message to the user
		else if (msg.getParam(0)[0] != '#') {
			for (int i = 0; i < (int)_users.size(); i++) {
				if (_users[i].getNickName() == msg.getParam(0))
					sendClient(user->getSd(), PRIVMSG(user->getNickName(), msg.getParam(0), msg.getParam(1)));
			}
		}
	}
}

void	Server::connectServer( int sd, User *user) {

	std::string response;
	
	char buffer[80];
	strftime(buffer, 40, "%a %b %d %H:%M:%S %Y", localtime(&_creationDate));
	std::string creationDate(buffer);

	sendClient(sd, RPL_WELCOME(user->getNickName()));
	sendClient(sd, RPL_YOURHOST(user->getNickName()));
	sendClient(sd, RPL_CREATED(user->getNickName(), creationDate));
	sendClient(sd, RPL_MYINFO(user->getNickName()));
	sendClient(sd, RPL_ISUPPORT(user->getNickName()));
	sendClient(sd, RPL_ISUPPORT2(user->getNickName()));
	sendClient(sd, RPL_LUSERCLIENT(user->getNickName(), "1", "1", "1"));
	sendClient(sd, RPL_LUSEROP(user->getNickName(), "1"));
	sendClient(sd, RPL_LUSERCHANNELS(user->getNickName(), "1"));
	sendClient(sd, RPL_LUSERME(user->getNickName(), "1", "1"));
	sendClient(sd, RPL_LOCALUSERS(user->getNickName(), "1", "1"));
	sendClient(sd, RPL_GLOBALUSERS(user->getNickName(), "1", "1"));
	sendClient(sd, RPL_MOTDSTART(user->getNickName()));
	sendClient(sd, RPL_MOTD(user->getNickName(), "Welcome to the Internet Relay Network " + user->getNickName()));
	sendClient(sd, RPL_MOTDEND(user->getNickName()));
}
