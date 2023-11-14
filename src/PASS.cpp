
#include "../includes/Server.hpp"

void	Server::passCmd( Message msg, User *user ) {
	if (msg.getParam(0).length() > 0) {
		user->setPass(msg.getParam(0));
		if(user->getPass() == _passwd)
			user->setPassOK(true);
		else
			user->setPassOK(false);
	}
	if (user->isNickNameSet() && user->isRealNameSet() && user->isPassOK()) {
			user->setRegistered(true);
			connectServer(user->getSd(), user);
	}
}


