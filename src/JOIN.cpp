
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

	if (channel[0] != '#') {
		sendClient(sd, ERR_NOSUCHCHANNEL(userNick, channel));
		return ;
	}

	if (_channels.find(channel) == _channels.end()) {
		this->createChannel(channel, userNick);
		create = true;
	}

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
	if (!_channels[channel]->getTopic().empty())
		sendClient(sd, RPL_TOPIC(userNick, channel, _channels[channel]->getTopic()));
	
	if (create) {
		std::time_t rawTime;
		std::time(&rawTime);
		std::stringstream ss;
		ss << rawTime;
		std::string Time = ss.str();
		std::string		irc = "FT_IRC";

		sendClient(sd, JOIN(userNick, channel));
		_channels[channel]->setTimeCreation(Time);
		_channels[channel]->sendMessgeToAllUsers(MODE(irc, channel, "+o", ""));
		std::string opNick = _channels[channel]->getChanNick(userNick);
		sendClient(sd, RPL_NAMREPLY(opNick, "=", channel, "", opNick));
		sendClient(sd, RPL_ENDOFNAMES(opNick, channel));
		sendClient(sd, RPL_CREATIONTIME(opNick, channel, Time));
	} else {
		std::map<std::string, int>::iterator it2;
		for(it2 = _channels[channel]->usersSd.begin(); it2 != _channels[channel]->usersSd.end(); ++it2){
			std::string nickOp = _channels[channel]->getChanNick(it2->first);
			sendClient(sd, RPL_NAMREPLY(userNick, "=", channel, "", nickOp));
			sendClient(it2->second, JOIN(userNick, channel));
		}
		sendClient(sd, RPL_ENDOFNAMES(userNick, channel));
		sendClient(sd, RPL_CREATIONTIME(userNick, channel, _channels[channel]->getTimeCreation()));
		sendClient(sd, MODE(userNick, channel, _channels[channel]->getCurrentModes(), ""));
	}
}
