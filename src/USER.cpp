
#include "../includes/Server.hpp"

void	Server::userCmd( Message msg, User *user ) {
	
	if (!msg.getParam(0).length())
		sendClient(user->getSd(), ERR_NEEDMOREPARAMS(user->getNickName(), msg.getCommand()));
	if (user->isRegistered() || user->isRealNameSet())
		sendClient(user->getSd(), ERR_ALREADYREGISTERED(user->getNickName()));
	if (user->isPassOK() == false)
		sendClient(user->getSd(), ERR_PASSWDMISMATCH(user->getNickName()));
	else {
		user->setUserName(msg.getParam(0));
		user->setRealName(msg.getParam(3));
		user->setRealNameSet(true);
	}
	if (user->isNickNameSet() && user->isPassOK()) {
			user->setRegistered(true);
			std::cout << "user registered dans USER" << std::endl;
			connectServer(user->getSd(), user);
	}
}

