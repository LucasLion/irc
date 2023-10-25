
#pragma once

#include "header.hpp"
#include "Message.hpp"
#include "Channel.hpp"

class User {

    public : 

							        User();
    void                	        printCommands( void );
    void                	        getBuffer( char * buf );
	void							setRealName( std::string realName );
	void							setNickName( std::string nickName );
    void                            setUserName(std::string userName);
    void                            setSd(int sd);
    int                             getSd();
	std::string						getRealName( void );
    std::string                     getUserName();
	std::string						getNickName( void );
	std::map<std::string, Channel>	getChannels( void );

	void							addChannel( std::string channelName );
    std::vector<Message>			messages;
    bool                            isRegistered();
    void                            setRegistered(bool isRegistered);
    bool                            isNickNameSet();
    bool                            isRealNameSet();
    void                            setNickNameSet(bool isNickNameSet);
    void                            setRealNameSet(bool isRealNameSet);


    private : 
    std::string					        _realName;
    std::string          		        _nickName;
    std::string                         _username;
    std::string                         _buffer;
	std::map<std::string, Channel>      _channels;
    int                                 _sd;
    bool                                _isRegistered;
    bool                                _isNickNameSet;
    bool                                _isRealNameSet;
};



