
#include "../includes/Server.hpp"

void	Server::whoCmd( Message msg, User* user ) {
    std::string userNick = user->getNickName();
    std::string mask = msg.getParam(0);
    if (mask.length() == 0) {
        sendClient(user->getSd(), ERR_NEEDMOREPARAMS(userNick, msg.getCommand()));
        return ;
    }
    if(mask[0] == '#') {
        if (_channels.find(mask) == _channels.end()) {
            sendClient(user->getSd(), ERR_NOSUCHCHANNEL(userNick, mask));
            return ;
        }
        std::map<std::string, int>::iterator it;
        for (it = _channels[mask]->usersSd.begin(); it != _channels[mask]->usersSd.end(); ++it) {
            std::string nick = it->first;
            for(std::vector<User>::iterator it2 = _users.begin(); it2 != _users.end(); ++it2){
                if (it2->getNickName() == nick)
                    sendClient(user->getSd(), RPL_WHOREPLY(userNick, mask, it2->getUserName(), nick, it2->getRealName()));
            }
        }
        std::string nickOp = _channels[mask]->getChanNick(userNick);
        sendClient(user->getSd(), RPL_ENDOFWHO(nickOp, mask));
    }
}
