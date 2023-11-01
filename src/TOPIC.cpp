
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
	// if protected topic (+t) and client does not have permissions
	if (_channels[channel]->isTopicProtected && !(_channels[channel]->isUserOp(userNick))) {
		sendClient(user->getSd(), ERR_CHANOPRIVSNEEDED(user->getNickName(), msg.getParam(0)));
		return ;
	}
	time_t rawDate;
	rawDate = time(NULL);
	char buffer[20];
	strftime(buffer, 20, "%a %b %d %H:%M:%S %Y", localtime(&rawDate));
	std::string creationDate(buffer);
	
	std::cout << "creationDate: " << creationDate << std::endl;

	_channels[channel]->setTopic(msg.getParam(1));
	std::map<std::string, int>::iterator it;
	for(it = _channels[channel]->usersSd.begin(); it != _channels[channel]->usersSd.end(); ++it) {
		std::string everyusernick = it->first;
		if (_channels[channel]->getTopic().empty()) {
			std::cout << "topic is empty" << std::endl;
			sendClient(it->second, RPL_NOTOPIC(everyusernick, channel));
		}
		else {
			sendClient(it->second, RPL_TOPIC(everyusernick, channel, msg.getParam(1)));
			sendClient(it->second, TOPIC(userNick, channel, msg.getParam(1)));

			// TODO fonctionne mal
		//	sendClient(it->second, RPL_TOPICWHOTIME(everyusernick, channel, userNick, creationDate));
		}
	}
}

