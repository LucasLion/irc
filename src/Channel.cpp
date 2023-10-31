
#include "../includes/header.hpp"
#include "../includes/Channel.hpp"

Channel::Channel( void ) {
    isInviteOnly = false;
    isTopicProtected = false;
    hasUserLimit = false;
    hasPassword = false;
 }

Channel::Channel( std::string nom ) : name(nom) { 
    isInviteOnly = false;
    isTopicProtected = false;
    hasUserLimit = false;
    hasPassword = false;
    (void)name; }

Channel::~Channel( void ) { }

const char*	Channel::ChannelAlreadyExistsException::what() const throw() {
	return (ERROR("Channel already exists"));
}

void	Channel::addUser( User* user ) {
    usersSd[user->getNickName()] = user->getSd();
}

bool	Channel::isUserInChannel( std::string user ) {
	
    if (usersSd.find(user) != usersSd.end())
        return (true);
    return (false);
}

void Channel::removeUser( std::string user ) {
    usersSd.erase(user);
}		

bool Channel::isUserOp( std::string user ) {
	for (std::vector<std::string>::iterator it = operList.begin(); it != operList.end(); ++it) {
		if (*it == user)
			return (true);
	}
	return (false);
}


void Channel::addOperator(std::string user) {
    operList.push_back(user);
}

void Channel::addInvite(std::string user) {
    inviteList.push_back(user);
}

void Channel::removeOperator(std::string user) {
    for (std::vector<std::string>::iterator it = operList.begin(); it != operList.end(); ++it) {
        if (*it == user) {
            operList.erase(it);
            return;
        }
    }
}

void Channel::removeInvite(std::string user) {
    for (std::vector<std::string>::iterator it = inviteList.begin(); it != inviteList.end(); ++it) {
        if (*it == user) {
            inviteList.erase(it);
            return;
        }
    }
}    

bool Channel::isUserInvite( std::string user ) {
    for (std::vector<std::string>::iterator it = inviteList.begin(); it != inviteList.end(); ++it) {
        if (*it == user)
            return (true);
    }
    return (false);
}


std::string Channel::getCurrentModes() {
    std::string modes;

	modes += "o";

    if (isInviteOnly) 
        modes += "i";
    
    if (isTopicProtected) 
        modes += "t";
    
    if (hasUserLimit) {
        modes += "l";
    }
	if (hasPassword)
		modes+= "k";

    return modes;
}


std::string	Channel::getTopic( void ) const {
	return (_topic);
}

void	Channel::setTopic( std::string topic ) {
	_topic = topic;
}


