
#include "../includes/Server.hpp"

void	Server::joinCmd( Message msg, User *user ) {
	
	int sd = user->getSd();
	std::string userNick = user->getNickName();
	std::string channel = msg.getParam(0);

	if (channel.length() == 0) {
		sendClient(sd, ERR_NEEDMOREPARAMS(userNick, msg.getCommand()));
		return ;
	}
	// check if the channel name is valid
	if (channel[0] != '#') {
		sendClient(sd, ERR_NOSUCHCHANNEL(userNick, channel));
		return ;
	}
	// check if the channel exists and create it if not
	if (_channels.find(channel) == _channels.end()) {
		this->createChannel(channel, userNick);
		//user->addChannel(channel, , &(_channels[channel])));
	}
	// check if the user is already in the channel
	if (_channels[channel]->isUserInChannel(userNick)) {
		sendClient(sd, ERR_USERONCHANNEL(userNick, userNick, channel));
		return ;
	}
	if (_channels[channel]->isInviteOnly) {
		if (!_channels[channel]->isUserInvite(userNick)) {
			sendClient(sd, ERR_INVITEONLYCHAN(userNick, channel));
			return ;
		}
	}
	_channels[channel]->addUser(user);
	user->addChannel(channel, _channels[channel]);
	sendClient(sd, JOIN(userNick, channel));
	// channel's topic
	std::cout << "Channel's topic: " << _channels[channel]->getTopic() << std::endl;
	if (_channels[channel]->getTopic().empty())
		sendClient(sd, RPL_NOTOPIC(userNick, channel));
	else {
		sendClient(sd, RPL_TOPIC(userNick, channel, _channels[channel]->getTopic()));
		// TODO fonctionne mal
		sendClient(sd, RPL_TOPICWHOTIME(userNick, channel, userNick, "creationDate"));
	}

	//sendClient(sd, RPL_TOPIC(userNick, channel, "default topic (has to change)"));
	// create the list of users in the channels
	std::string response = ":localhost 353 " + userNick + " = " + channel + " :";
	std::map<std::string, int>::iterator it;
	
	for(it = _channels[channel]->usersSd.begin(); it != _channels[channel]->usersSd.end(); ++it) {
		response += it->first + " ";
	}
	response += "\r\n";
	// send the list to everyone in the channel
	for(it = _channels[channel]->usersSd.begin(); it != _channels[channel]->usersSd.end(); ++it) {
		sendClient(it->second, JOIN(userNick, channel));
		send(it->second, response.c_str(), response.length(), 0);
		sendClient(it->second, RPL_ENDOFNAMES(userNick, channel));
		
	}

	// loop with iterator to send the list to everyone in the channels
	// std::vector<User*>::iterator it;
	// for (it = _channels[channel]->userList.begin(); it != _channels[channel]->userList.end(); ++it) {
	// 	sendClient((*it)->getSd(), RPL_NAMREPLY(userNick, "=", channel, "", (*it)->getNickName()));
	// 	sendClient(sd, RPL_ENDOFNAMES(userNick, channel));
	// };

}
