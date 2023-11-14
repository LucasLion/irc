
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
		int 						getNbUsers( void );
		bool						isUserInChannel( std::string user );
		bool						isUserOp( std::string user );
		bool						isUserInvite( std::string user );
		std::string					getCurrentModes ( void );
		void						addOperator (std::string user );
		void						addInvite (std::string user );
		void 						removeOperator(std::string user);
		void						removeInvite(std::string user);
		std::string					getTopic( void ) const;
		void						setTopic( std::string topic );
		std::string					getTimeCreation( void ) const;
		void						setTimeCreation( std::string time );
		void						sendMessgeToAllUsers( std::string message );
		std::string					getChanNick( std::string user );
		void						changeNick( std::string prevNick, std::string newNick );	

		
		
		bool								isInviteOnly;
		bool								isTopicProtected;
		bool								hasUserLimit;
		bool								hasPassword;
		unsigned long						userLimit;
		std::string							name;
		std::vector<std::string>			operList;
		std::vector<std::string>			inviteList;
		std::string							password;
		std::map<std::string, int>			usersSd;

	private:

		std::string					_topic;
		std::string					_timeCreation;
};

