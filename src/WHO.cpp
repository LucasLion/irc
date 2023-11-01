#include "../includes/Server.hpp"

void	Server::whoCmd( Message msg, User* user ) {
    std::string userNick = user->getNickName();
    std::string mask = msg.getParam(0);
    if (mask.length() == 0) {
        sendClient(user->getSd(), ERR_NEEDMOREPARAMS(userNick, msg.getCommand()));
        return ;
    }
    if(mask[0] == '#'){
        if (_channels.find(mask) == _channels.end()) {
            sendClient(user->getSd(), ERR_NOSUCHCHANNEL(userNick, mask));
            return ;
        }
        std::map<std::string, int>::iterator it;
        for (it = _channels[mask]->usersSd.begin(); it != _channels[mask]->usersSd.end(); ++it) {
            std::string nickop = _channels[mask]->getChanNick(it->first);
            sendClient(it->second, RPL_WHOREPLY(nickop, mask, user->getUserName(), nickop, user->getRealName()));
        }
        std::string opnick = _channels[mask]->getChanNick(userNick);
        sendClient(user->getSd(), RPL_ENDOFWHO(opnick, mask));

    }
}