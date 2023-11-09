#include "../includes/header.hpp"
#include "../includes/Server.hpp"

bool Server::is_valid( const std::string nickname ) {
	if (nickname.length() < 1 || nickname.length() > 32)
        return false;
    const std::string validChars = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789_[]{}\\|";
    for (size_t i = 0; i < nickname.length(); ++i) {
        char c = nickname[i];
		// ajout de carateres ici
        if (i == 0 && !isalpha(c) && c != '[' && c != '{' && c != '\\' && c != '|' && c != ']' && c != '}' && c != '_')
            return false;
        if (i > 0 && validChars.find(c) == std::string::npos)
            return false;
    }
    return true;
}

void	Server::sendClient( int sd, std::string response ) {
	std::cout << "COMMAND_SENT: " << response; 
	write(sd, response.c_str(), response.length());
}

std::string		Server::generateDefaultNick( void ) {
	int				numGuest = 1;
	char			num[100];
	std::string		defaultNick;
	std::vector<User>::iterator it;

	sprintf(num, "%c", numGuest);
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
		return;
	}
	new_nick = msg.getParam(0);
	for (it = _users.begin(); it != _users.end(); ++it) {
		if (new_nick == it->getNickName()) {
			sendClient(sd, ERR_NICKNAMEINUSE(msg.getParam(0), new_nick));
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

bool	Server::isUserInServer( std::string nickname ) {
	std::vector<User>::iterator it;

	for (it = _users.begin(); it != _users.end(); ++it) {
		if (nickname == it->getNickName())
			return (true);
	}
	return (false);
}

int		Server::getUserSd( std::string nickname ) {
	std::vector<User>::iterator it;

	for (it = _users.begin(); it != _users.end(); ++it) {
		if (nickname == it->getNickName())
			return (it->getSd());
	}
	return (-1);
}

std::string	Server::currentDate() {
	time_t rawDate;
	rawDate = time(NULL);
	char buffer[90];
	strftime(buffer, 90, "%a %b %d %H:%M:%S %Y", localtime(&rawDate));
	std::string creationDate(buffer);
	return creationDate;
}

//void	Server::sigint( int sig ) {
//	std::cout << "Signal" << std::endl;
//}

