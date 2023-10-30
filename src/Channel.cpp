
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
	std::cout << "User " << user->getNickName() << " added to channel " << name << std::endl;
}

bool	Channel::isUserInChannel( std::string user ) {
	for (std::vector<User*>::iterator it = userList.begin(); it != userList.end(); it++) {
		if ((*it)->getNickName() == user)
			return (true);
	}
	return (false);
}

void Channel::removeUser(User* user) {
    for (std::vector<User*>::iterator it = userList.begin(); it != userList.end(); ++it) {
        if (*it == user) {
			std::cout << "User " << user->getNickName() << " removed from channel " << name << std::endl;
            userList.erase(it);
            return;
    	}
	}
}			

