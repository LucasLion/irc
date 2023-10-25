
#pragma once

#include "header.hpp"
#include "User.hpp"

class User;
class Channel {

	public:
									Channel( void );
									Channel( std::string name );
									~Channel( void );
		void						addUser( User* user );
		bool						isUserInChannel( std::string user );

		std::string					name;
		std::vector<User*>			userList;

		class ChannelAlreadyExistsException : public std::exception {
				virtual const char*	what() const throw();
		};
	
	private:
};
