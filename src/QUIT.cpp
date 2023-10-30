
#include "../includes/Server.hpp"

void Server::quitCmd(Message msg, User *user) {

	std::string reason = "Quit: ";

	if (msg.getParam(0).length()> 0)
		reason += msg.getParam(0);
    for (std::vector<User>::iterator it = _users.begin(); it != _users.end(); ++it) {
        if (it->getNickName() != user->getNickName()) 
            sendClient(it->getSd(),QUIT(user->getNickName(), reason));
    }

	if (user->_channels.size() > 0) {
		for (std::map<std::string, Channel*>::iterator it = user->_channels.begin(); it != user->_channels.end(); ++it) {
			for (int i = 0 ; i < (int)it->second->userList.size(); i++) {
				sendClient(it->second->userList[i]->getSd(), QUIT(user->getNickName(), reason));
				it->second->removeUser(user->getNickName());
			}
		}
	}else
		sendClient(user->getSd(),QUIT(user->getNickName(), reason));
}

