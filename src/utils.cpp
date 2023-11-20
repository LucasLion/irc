#include "../includes/header.hpp"
#include "../includes/Server.hpp"

bool Server::is_valid( const std::string nickname ) {
	if (nickname.length() < 1 || nickname.length() > 32)
        return false;
    const std::string validChars = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789_[]{}\\|";
    for (size_t i = 0; i < nickname.length(); ++i) {
        char c = nickname[i];
        if (i == 0 && !isalpha(c) && c != '[' && c != '{' && c != '\\' && c != '|' && c != ']' && c != '}' && c != '_')
            return false;
        if (i > 0 && validChars.find(c) == std::string::npos)
            return false;
    }
    return true;
}

void	Server::sendClient( int sd, std::string response ) {
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
	if (!is_valid(new_nick)) 
			new_nick = generateDefaultNick();
	user->setNickName(new_nick);
	user->setNickNameSet(true);
	if (user->isPassOK() == false)
		sendClient(user->getSd(), ERR_PASSWDMISMATCH(user->getNickName()));
	if(user->isPassOK() && user->isRealNameSet()){
		user->setRegistered(true);
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


void 	Server::disconnectChannels( std::string userNick ){
	std::map<std::string, Channel*>::iterator channelit;
	for (channelit = _channels.begin(); channelit != _channels.end(); ++channelit) {
		Channel* channel = channelit->second;
		if (channel->isUserInChannel(userNick)) {
			std::string chanName = channel->name;
			std::string reason = "User disconnected";
            for(std::map<std::string, int>::iterator userit = channel->usersSd.begin(); userit != channel->usersSd.end(); ++userit){
			    std::string nickOp = channel->getChanNick(userit->first);
			   	sendClient(userit->second, PART(userNick, chanName, reason));
            }
            channel->removeUser(userNick);
            if (channel->usersSd.size() == 0) {
                delete channel;
                _channels.erase(chanName);
            }

		}
	}
}