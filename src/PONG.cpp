
#include "../includes/Server.hpp"

void	Server::pongCmd( Message msg, User *user ) {
	sendClient(user->getSd(), PONG(msg.getParam(0)));
}

