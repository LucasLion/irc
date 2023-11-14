
#include "../includes/Server.hpp"

void	Server::topicCmd( Message msg, User *user ) {

	int			sd = user->getSd();
	std::string response = "";
	std::string userNick = user->getNickName();
	std::string channel = msg.getParam(0);

	if (channel.length() == 0) {
		sendClient(sd, ERR_NEEDMOREPARAMS(userNick, msg.getCommand()));
		return ;
	}
	if (_channels.find(channel) == _channels.end()) {
		sendClient(sd, ERR_NOTONCHANNEL(userNick, channel));
		return ;
	}
	if (_channels[channel]->isTopicProtected && !(_channels[channel]->isUserOp(userNick))) {
		sendClient(user->getSd(), ERR_CHANOPRIVSNEEDED(user->getNickName(), msg.getParam(0)));
		return ;
	}
	
	_channels[channel]->setTopic(msg.getParam(1));
	std::map<std::string, int>::iterator it;
	for(it = _channels[channel]->usersSd.begin(); it != _channels[channel]->usersSd.end(); ++it) {
		std::string everyusernick = it->first;
		if (_channels[channel]->getTopic().empty()) {
			sendClient(it->second, RPL_NOTOPIC(everyusernick, channel));
		}
		else {
			sendClient(it->second, RPL_TOPIC(everyusernick, channel, msg.getParam(1)));
			sendClient(it->second, TOPIC(userNick, channel, msg.getParam(1)));
		}
	}
}

