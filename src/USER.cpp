
#include "../includes/Server.hpp"

void	Server::userCmd( Message msg, User *user ) {
	
	if (msg.nbParam() < 4 || msg.nbParam() > 5){
		sendClient(user->getSd(), ERR_NEEDMOREPARAMS(user->getNickName(), msg.getCommand()));
		return;
	}

	if (user->isRegistered() || user->isRealNameSet()){
		sendClient(user->getSd(), ERR_ALREADYREGISTERED(user->getNickName()));
		return;
	}
	user->setUserName(msg.getParam(0));
	user->setRealName(msg.getParam(3));
	user->setRealNameSet(true);
	if (user->isPassOK() == false)
		sendClient(user->getSd(), ERR_PASSWDMISMATCH(user->getNickName()));
	
	if (user->isNickNameSet() && user->isPassOK()) {
			user->setRegistered(true);
			connectServer(user->getSd(), user);
	}
}

