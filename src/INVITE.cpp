
#include "../includes/Server.hpp"

void	Server::inviteCmd( Message msg, User* user ) {
	
	int sd = user->getSd();
	std::string userNick = user->getNickName();

	if (msg.nbParam() < 2) {
		sendClient(sd, ERR_NEEDMOREPARAMS(userNick, msg.getCommand()));
		return ;
	}

	std::string channel = msg.getParam(0);
	std::string target = msg.getParam(1);

	if (channel[0] != '#' && target[0] == '#') {
		channel = msg.getParam(1);
		target = msg.getParam(0);
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

	if (_channels[channel]->isUserInChannel(target)) {
		sendClient(sd, ERR_USERONCHANNEL(userNick, target, channel));
		return ;
	}

	if (isUserInServer(target) == false) {
		sendClient(sd, ERR_NOSUCHNICK(userNick, target));
	}
	else {
		if (_channels[channel]->isUserOp(userNick)) {
			_channels[channel]->inviteList.push_back(target);
			sendClient(sd, RPL_INVITING(userNick, target, channel));
			sendClient(getUserSd(target), INVITE(userNick, target, channel));
		}
		else
			sendClient(sd, ERR_CHANOPRIVSNEEDED(userNick, channel));
	}
}
