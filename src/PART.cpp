#include "../includes/Server.hpp"

void	Server::partCmd( Message msg, User *user ) {
  
     if (msg.nbParam() == 0) {
        sendClient(user->getSd(), ERR_NEEDMOREPARAMS(user->getNickName(), msg.getCommand()));
        return ;
    }
    
    std::string chanName = msg.getParam(0);
    Channel *channel = _channels[chanName];
    std::string userNick = user->getNickName();
    std::string reason;
   

    if (msg.nbParam() == 1)
        reason = "No reason given";
    else
        reason = msg.getParam(1);


    if (_channels.find(chanName) != _channels.end()) {
        if (channel->isUserInChannel(userNick)) {
            for(std::map<std::string, int>::iterator it = channel->usersSd.begin(); it != channel->usersSd.end(); ++it){
			    std::string nickOp = channel->getChanNick(it->first);
			   	sendClient(it->second, PART(userNick, chanName, reason));
            }
            channel->removeUser(userNick);
            user->removeChannel(chanName);      
            if (channel->usersSd.size() == 0) {
                delete channel;
                _channels.erase(chanName);
            }
        } else {
            sendClient(user->getSd(), ERR_NOTONCHANNEL(userNick, chanName));
        }
    } else {
        sendClient(user->getSd(), ERR_NOSUCHCHANNEL(userNick, chanName));
    }
    
  }