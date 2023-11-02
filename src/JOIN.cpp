
#include "../includes/Server.hpp"

void	Server::joinCmd( Message msg, User *user ) {
	
	int sd = user->getSd();
	std::string userNick = user->getNickName();
	std::string channel = msg.getParam(0);
	bool create = false;

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
		create = true;
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
	if (_channels[channel]->hasUserLimit && _channels[channel]->usersSd.size() >= _channels[channel]->userLimit) {
			sendClient(sd, ERR_CHANNELISFULL(userNick, channel));
		return ;
	}

	if(_channels[channel]->hasPassword) {
		if (msg.nbParam() < 2 || msg.getParam(1).length() == 0 || (msg.getParam(1) != _channels[channel]->password) ) {
			sendClient(sd, ERR_BADCHANNELKEY(userNick, channel));
			return ;
		}
	}
	_channels[channel]->addUser(user);
	user->addChannel(channel, _channels[channel]);
	// channel's topic
	std::cout << "Channel's topic: " << _channels[channel]->getTopic() << std::endl;
	if (_channels[channel]->getTopic().empty())
		sendClient(sd, RPL_NOTOPIC(userNick, channel));
	else {
		sendClient(sd, RPL_TOPIC(userNick, channel, _channels[channel]->getTopic()));
		// TODO fonctionne mal
		//sendClient(sd, RPL_TOPICWHOTIME(userNick, channel, userNick, "creationDate"));
	}

	
	if(create){
		sendClient(sd, JOIN(userNick, channel));
		sendClient(sd, MODE(userNick, channel, "+o", ""));
		std::string opNick = _channels[channel]->getChanNick(userNick);
		sendClient(sd, RPL_NAMREPLY(opNick, "=", channel, "", opNick));
		sendClient(sd, RPL_ENDOFNAMES(opNick, channel));
	}else{
		std::cout << "else"	<< std::endl;

		std::map<std::string, int>::iterator it2;
		for(it2 = _channels[channel]->usersSd.begin(); it2 != _channels[channel]->usersSd.end(); ++it2){
			std::string nickOp = _channels[channel]->getChanNick(it2->first);
			sendClient(sd, RPL_NAMREPLY(userNick, "=", channel, "", nickOp));
		}
		std::map<std::string, int>::iterator it;
		for(it = _channels[channel]->usersSd.begin(); it != _channels[channel]->usersSd.end(); ++it) {
			sendClient(it->second, RPL_ENDOFNAMES(userNick, channel));
			sendClient(it->second, JOIN(userNick, channel));
			sendClient(sd, MODE(userNick, channel, "+", _channels[channel]->getCurrentModes()));
		}
	}
}
