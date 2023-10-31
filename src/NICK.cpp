
#include "../includes/Server.hpp"

void	Server::nickCmd( Message msg, User *user ) {
	
	int sd = user->getSd();
	std::string newNick = msg.getParam(0);
	std::string oldNick = user->getNickName();

	std::vector<User>::iterator it;
	std::map<std::string, Channel*>::iterator it2;

	if (newNick.length() == 0) {
		sendClient(sd, ERR_NONICKNAMEGIVEN(user->getNickName()));
		return;
	}
	for (it = _users.begin(); it != _users.end(); ++it) {
		if (newNick == it->getNickName()) {
			sendClient(sd, ERR_NICKNAMEINUSE(user->getNickName(), newNick));
			return;
		}
	}
	if (!is_valid(newNick)) {
		sendClient(sd, ERR_ERRONEUSNICKNAME(user->getNickName(), newNick));
		return;
	}
	user->setNickName(newNick);
	if (user->_channels.size() > 0){
		for (it2 = user->_channels.begin(); it2 != user->_channels.end(); ++it2) {
			std::map<std::string, int>::iterator it3;
			for(it3 = it2->second->usersSd.begin(); it3 != it2->second->usersSd.end(); ++it3) {
				sendClient(it3->second, NICK(oldNick, newNick));
			}
		}
	}else
		sendClient(sd, NICK(oldNick, newNick));
}
