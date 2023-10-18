
#include "../includes/header.hpp"
#include "../includes/Channel.hpp"

Channel::Channel( void ) { }

Channel::Channel( std::string nom ) : name(nom) { (void)name; }

Channel::~Channel( void ) { }

const char*	Channel::ChannelAlreadyExistsException::what() const throw() {
	return (ERROR("Channel already exists"));
}

void	Channel::addUser( std::string user ) {
	userList.push_back(user);
	std::cout << "User " << user << " added to channel " << name << std::endl;
}

bool	Channel::isUserInChannel( std::string user ) {
	for (std::vector<std::string>::iterator it = userList.begin(); it != userList.end(); it++) {
		if (*it == user) {
			std::cout << "User " << user << " is in channel " << name << std::endl;
			return (true);
		}
	}
	std::cout << "User " << user << " is not in channel " << name << std::endl;
	return (false);
}
