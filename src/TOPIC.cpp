
#include "../includes/Server.hpp"

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

