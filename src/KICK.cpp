
#include "../includes/Server.hpp"

void	Server::kickCmd( Message msg, User* user ) {
	
	int sd = user->getSd();
	std::string userNick = user->getNickName();
	std::string channel = msg.getParam(0);
	std::string target = msg.getParam(1);
	std::string reason = msg.getParam(2);

	if (channel.length() == 0 || target.length() == 0) {
		sendClient(sd, ERR_NEEDMOREPARAMS(userNick, msg.getCommand()));
		return ;
	}

	if (channel[0] != '#') {
		sendClient(sd, ERR_NOSUCHCHANNEL(userNick, channel));
		return ;
	}

	if (_channels.find(channel) == _channels.end()) {
		sendClient(sd, ERR_NOSUCHCHANNEL(userNick, channel));
		return ;
	}

	if (!_channels[channel]->isUserInChannel(userNick)) {
		sendClient(sd, ERR_NOTONCHANNEL(userNick, channel));
		return ;
	}

	if (!_channels[channel]->isUserInChannel(target)) {
		sendClient(sd, ERR_USERNOTINCHANNEL(userNick, target, channel));
		return ;
	}

	if (_channels[channel]->isUserOp(userNick)) {
		int targetSd = _channels[channel]->usersSd[target];
		_channels[channel]->removeUser(target);
		if (reason.length() == 0)
			reason = "No reason given";
		sendClient(targetSd, KICK(userNick, channel, target, reason));
		std::map<std::string, int>::iterator it;
		for(it = _channels[channel]->usersSd.begin(); it != _channels[channel]->usersSd.end(); ++it) 
			sendClient(it->second, KICK(userNick, channel, target, reason));
	}
	else
		sendClient(user->getSd(), ERR_CHANOPRIVSNEEDED(user->getNickName(), msg.getParam(0)));
}
