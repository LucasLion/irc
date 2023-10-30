
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
		void 						removeUser( std::string user );
		bool						isUserInChannel( std::string user );
		bool						isUserOp( std::string user );

		std::string					getTopic( void ) const;
		void						setTopic( std::string topic );
		User*						getUser( std::string user );

		
		std::string					name;
		std::vector<User*>			userList;
		std::vector<std::string>	operList;
		std::vector<std::string>	bannedList;

		class ChannelAlreadyExistsException : public std::exception {
				virtual const char*	what() const throw();
		};
	
	private:

		std::string					_topic;
};

