
#pragma once

#include "header.hpp"
#include "User.hpp"

class Channel {

	public:
									Channel( void );
									Channel( std::string name );
									~Channel( void );

		std::string					name;
		std::vector<std::string>	userList;
		User						_users[30];

		class ChannelAlreadyExistsException : public std::exception {
				virtual const char*	what() const throw();
		};
	
	private:
};
