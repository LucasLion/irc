
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
	// check if the target is in the channel
	if (!_channels[channel]->isUserInChannel(target)) {
		sendClient(sd, ERR_USERNOTINCHANNEL(userNick, target, channel));
		return ;
	}
	// kick the target
	int targetSd = _channels[channel]->usersSd[target];
	_channels[channel]->removeUser(target);
	// send the kick message to the target
	//
	//
	// get the target's sd
	

	sendClient(targetSd, KICK(userNick, channel, target, reason));
	// send the kick message to everyone in the channel
	std::map<std::string, int>::iterator it;
	for(it = _channels[channel]->usersSd.begin(); it != _channels[channel]->usersSd.end(); ++it) {
		sendClient(it->second, KICK(userNick, channel, target, reason));
	}
}
