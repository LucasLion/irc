
#include "../includes/Server.hpp"

void	Server::prvMsgCmd( Message msg, User *user ) {

	std::string userNick = user->getNickName();
	std::string reciever = msg.getParam(0);
	std::string message = msg.getParam(1);

	if (reciever.length() == 0) {
		sendClient(user->getSd(), ERR_NORECIPIENT(userNick));
		return ;
	}
	if (message.length() == 0) {
		sendClient(user->getSd(), ERR_NOTEXTTOSEND(userNick));
		return ;
	}
	if (user->getChannels().find(reciever) == user->getChannels().end()) {
		sendClient(user->getSd(), ERR_NOSUCHNICK(userNick, reciever));
		return ;
	}
	// send the message to all the users in the channel
	if (reciever[0] == '#') {
		// segfault quand on essaye d'acceder a un salon qui n'existe pas !!!!!!!
		std::map<std::string, int>::iterator it;	
		for (it =_channels[reciever]->usersSd.begin(); it != _channels[reciever]->usersSd.end(); ++it) {
			if (it->first != userNick) {
				sendClient(it->second, PRIVMSG(userNick, reciever, message));
			}
		}
	}
	// send private message to the user
	else if (reciever[0] != '#') {
		for (int i = 0; i < (int)_users.size(); i++) {
			if (_users[i].getNickName() != userNick && _users[i].getNickName() == reciever) {
				sendClient(_users[i].getSd(), PRIVMSG(userNick, reciever, message));
			}
		}
	}
}

