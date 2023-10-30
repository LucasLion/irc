
#include "../includes/header.hpp"
#include "../includes/Channel.hpp"

Channel::Channel( void ) { }

Channel::Channel( std::string nom ) : name(nom) { (void)name; }

Channel::~Channel( void ) { }

const char*	Channel::ChannelAlreadyExistsException::what() const throw() {
	return (ERROR("Channel already exists"));
}

void	Channel::addUser( User* user ) {
	userList.push_back(user);
}

bool	Channel::isUserInChannel( std::string user ) {
	for (std::vector<User*>::iterator it = userList.begin(); it != userList.end(); it++) {
		if ((*it)->getNickName() == user)
			return (true);
	}
	return (false);
}

void Channel::removeUser( std::string user ) {
    for (std::vector<User*>::iterator it = userList.begin(); it != userList.end(); ++it) {
        if ((*it)->getNickName() == user) {
            userList.erase(it);
            return;
    	}
	}
}			

bool Channel::isUserOp( std::string user ) {
	for (std::vector<std::string>::iterator it = operList.begin(); it != operList.end(); it++) {
		if (*it == user)
			return (true);
	}
	return (false);
}

std::string	Channel::getTopic( void ) const {
	return (_topic);
}

void	Channel::setTopic( std::string topic ) {
	_topic = topic;
}

User*	Channel::getUser( std::string user ) {
	for (std::vector<User*>::iterator it = userList.begin(); it != userList.end(); it++) {
		if ((*it)->getNickName() == user)
			return (*it);
	}
	return (NULL);
}
