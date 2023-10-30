#include "../includes/Server.hpp"

void	Server::modeCmd( Message msg, User *user ) {
	
	std::cout << "modeCmd : " << msg.rawMessage	<< std::endl;
	Channel *channel;
	std::map<std::string, Channel*>::iterator it;
	std::string nick = user->getNickName();
	std::string target = msg.getParam(0);
	int sd = user->getSd();
	int nbArgs = msg.nbParam() - 1;
	std::string modeArgs[nbArgs];
	for(int i = 0; i < nbArgs; i++)
		modeArgs[i] = msg.getParam(1 + i);
	
	if(target[0] != '#')
	 	sendClient(sd, ERR_UMODEUNKNOWNFLAG(nick));
	else {
		if(_channels.find(target) == _channels.end()){
			sendClient(sd, ERR_NOSUCHCHANNEL(nick, target));
			return;
		}
		if (nbArgs == 0){
	  		sendClient(sd, RPL_CHANNELMODEIS(nick, target, "+,-", "i,t,k,o,l"));
			return;
		}
		//find the channel
		for (it = getChannels()->begin(); it != getChannels()->end(); ++it) {
				if (it->first == target)
					channel = it->second;
				std::cout << "Channel: " << it->first << std::endl;
			}
		//check if the user is in the channel && is op
		if (channel->isUserInChannel(nick) == false){
			sendClient(sd, ERR_NOTONCHANNEL(nick, target));
			return;
		}
		if (channel->isUserOp(nick) == false){
			sendClient(sd, ERR_CHANOPRIVSNEEDED(nick, target));
			return;
		}
		

	// // else  
	// // 	sendClient(user->getSd(), "OOOOOOKKKKKKKK");
	}
}