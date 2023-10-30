
#include "../includes/header.hpp"
#include "../includes/Server.hpp"

bool Server::is_valid( const std::string nickname ) {
	if (nickname.length() < 1 || nickname.length() > 32)
        return false;
    const std::string validChars = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789_[]{}\\|";
    for (size_t i = 0; i < nickname.length(); ++i) {
        char c = nickname[i];
        if (i == 0 && !isalpha(c) && c != '[' && c != '{' && c != '\\' && c != '|')
            return false;
        if (i > 0 && validChars.find(c) == std::string::npos)
            return false;
    }
    return true;
}

void	Server::sendClient( int sd, std::string response ) {
	write(sd, response.c_str(), response.length());
}

std::string		Server::generateDefaultNick() {
	int				numGuest = 1;
	char			num[10];
	std::string		defaultNick;
	std::vector<User>::iterator it;

	sprintf(num, "%d", numGuest);
	defaultNick = "guest" + static_cast<std::string>(num);
	for (it = _users.begin(); it != _users.end(); ++it) {
		if (defaultNick == it->getNickName()) {
			numGuest++;
			sprintf(num, "%d", numGuest);
			defaultNick = "guest" + static_cast<std::string>(num);
			it = _users.begin();
		}
	}
	return (defaultNick);
}

void	Server::nickPreRegistration( Message msg, User *user ) {
	
	std::string new_nick;
	std::string old_nick;
	int sd = user->getSd();
	std::string response;
	std::vector<User>::iterator it;
	std::map<std::string, Channel*>::iterator it2;

	if (msg.getParam(0).length() == 0) {
		new_nick = generateDefaultNick();
		sendClient(sd, ERR_NONICKNAMEGIVEN(new_nick));
		//response = "we gave you the nickname : " + new_nick + "\r\n";
		//sendClient(sd, response);
		return;
	}

	new_nick = msg.getParam(0);
	for (it = _users.begin(); it != _users.end(); ++it) {
		if (new_nick == it->getNickName()) {
			//new_nick = generateDefaultNick();
			sendClient(sd, ERR_NICKNAMEINUSE(msg.getParam(0), new_nick));
			//response = "we gave you the nickname : " + new_nick + "\r\n";
			//sendClient(sd, response);
			return;
		}
	}

	if (!is_valid(new_nick)) {
			std::cout << "new_nick: " << new_nick << std::endl;
			std::cout << "size: " << new_nick.length() << std::endl;
			new_nick = generateDefaultNick();
			std::cout << "new_nick: " << new_nick << std::endl;
			//sendClient(sd, ERR_ERRONEUSNICKNAME(user->getNickName(), new_nick));
			//response = "we gave you the nickname : " + new_nick + "\r\n";
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