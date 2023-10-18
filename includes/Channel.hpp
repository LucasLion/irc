
#pragma once

#include "header.hpp"

class Channel {

	public:
									Channel( void );
									Channel( std::string name );
									~Channel( void );
		void						addUser( std::string user );
		bool						isUserInChannel( std::string user );

		std::string					name;
		std::vector<std::string>	userList;

		class ChannelAlreadyExistsException : public std::exception {
				virtual const char*	what() const throw();
		};
	
	private:
};
