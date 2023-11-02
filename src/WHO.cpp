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
            std::string nick = _channels[mask]->getChanNick(it->first);
            sendClient(it->second, RPL_WHOREPLY(nick, mask, user->getUserName(), nick, user->getRealName()));
        }
        std::string nickOp = _channels[mask]->getChanNick(userNick);
        sendClient(user->getSd(), RPL_ENDOFWHO(nickOp, mask));
    }
}
