
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
	if (_channels[channel]->isTopicProtected && !(_channels[channel]->isUserOp(userNick)))
		sendClient(user->getSd(), ERR_CHANOPRIVSNEEDED(user->getNickName(), msg.getParam(0)));

	time_t rawDate;
	rawDate = time(NULL);
	char buffer[80];
	strftime(buffer, 80, "%a %b %d %H:%M:%S %Y", localtime(&rawDate));
	std::string creationDate2(buffer);
	
	std::cout << "creationDate: " << creationDate2 << std::endl;

	_channels[channel]->setTopic(msg.getParam(1));
	for (int i = 0; i < (int)_channels[channel]->userList.size(); i++) {

		std::string everyusernick = _channels[channel]->userList[i]->getNickName();
		int userSd = _channels[channel]->userList[i]->getSd();
		std::cout << "everyusernick: " << everyusernick << std::endl;
		if (_channels[channel]->getTopic().empty()) {
			std::cout << "topic is empty" << std::endl;
			sendClient(userSd, RPL_NOTOPIC(everyusernick, channel));
		}
		else {
			sendClient(userSd, RPL_TOPIC(everyusernick, channel, msg.getParam(1)));

			// TODO fonctionne mal
			sendClient(userSd, RPL_TOPICWHOTIME(everyusernick, channel, userNick, creationDate2));
		}
	}
}

