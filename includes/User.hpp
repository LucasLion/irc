
#pragma once

#include "header.hpp"
#include "Message.hpp"
#include "Channel.hpp"

class Channel;

class User {

    public : 
										User( void );
		void                	        printCommands( void );
		void                	        getBuffer( char * buf );
		void							setRealName( std::string realName );
		void							setNickName( std::string nickName );
		void                            setUserName(std::string userName);
		void                            setSd( int sd );
		int                             getSd( void );
		std::string						getRealName( void );
		std::string                     getUserName( void );
		std::string                     getPass( void );
		std::string						getNickName( void );
		std::map<std::string, Channel*>	getChannels( void );

		void							addChannel( std::string channelName, Channel* channel );
		void							removeChannel( std::string channelName );
		bool                            isRegistered( void );
		bool                            isPassOK( void );
		void                            setRegistered( bool isRegistered );
		bool                            isNickNameSet( void );
		bool                            isRealNameSet( void );
		void                            setNickNameSet( bool isNickNameSet );
		void                            setRealNameSet( bool isRealNameSet );
		void                            setPass( std::string pass );
		void                            setPassOK( bool passOk );

		std::map<std::string, Channel*> _channels;
		std::vector<Message>			messages;

    private : 
		std::string				        _realName;
		std::string      		        _nickName;
		std::string                     _username;
		std::string                     _buffer;
		int                             _sd;
		bool                            _isRegistered;
		bool                            _isNickNameSet;
		bool                            _isRealNameSet;
		std::string                     _pass;
		bool                            _passOk;;
};



