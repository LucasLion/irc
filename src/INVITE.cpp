
#include "../includes/Server.hpp"

void	Server::inviteCmd( Message msg, User* user ) {
	
	int sd = user->getSd();
	std::string userNick = user->getNickName();
	std::string channel = msg.getParam(0);
	std::string target = msg.getParam(1);

	if (channel.length() == 0 || target.length() == 0) {
		sendClient(sd, ERR_NEEDMOREPARAMS(userNick, msg.getCommand()));
		return ;
	}
	// check if the channel name is valid
	if (channel[0] != '#') {
		sendClient(sd, ERR_NOSUCHCHANNEL(userNick, channel));
		return ;
	}
	// check if the channel exists
	if (_channels.find(channel) == _channels.end()) {
		sendClient(sd, ERR_NOSUCHCHANNEL(userNick, channel));
		return ;
	}
	// check if the user is in the channel
	if (!_channels[channel]->isUserInChannel(userNick)) {
		sendClient(sd, ERR_NOTONCHANNEL(userNick, channel));
		return ;
	}
	// check if the target is already invited
	if (_channels[channel]->isUserInChannel(target)) {
		sendClient(sd, ERR_USERONCHANNEL(userNick, target, channel));
		return ;
	}
	if (_channels[channel]->isUserOp(userNick)) {
		_channels[channel]->inviteList.push_back(target);
		int targetSd = _channels[channel]->usersSd[target];
		sendClient(sd, RPL_INVITING(userNick, target, channel));
		sendClient(targetSd, INVITE(userNick, target, channel));
	}
	else
		sendClient(sd, ERR_CHANOPRIVSNEEDED(userNick, target));
}
