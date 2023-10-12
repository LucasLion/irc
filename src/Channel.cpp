
#include "../includes/header.hpp"
#include "../includes/Channel.hpp"

Channel::Channel( void ) { }

Channel::Channel( std::string nom ) : name(nom) { (void)name; }

Channel::~Channel( void ) { }

const char*	Channel::ChannelAlreadyExistsException::what() const throw() {
	return (ERROR("Channel already exists"));
}


