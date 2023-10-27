
#pragma once

#include "header.hpp"
#include "Message.hpp"
#include "Channel.hpp"

class Channel;

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
    std::string                     getPass();
	std::string						getNickName( void );
	std::map<std::string, Channel*>	getChannels( void );

	void							addChannel( std::string channelName, Channel* channel );
    std::vector<Message>			messages;
    bool                            isRegistered();
    bool                            isPassOK();
    void                            setRegistered(bool isRegistered);
    bool                            isNickNameSet();
    bool                            isRealNameSet();
    void                            setNickNameSet(bool isNickNameSet);
    void                            setRealNameSet(bool isRealNameSet);
    void                            setPass(std::string pass);
    void                            setPassOK(bool passOK);
    std::map<std::string, Channel*> _channels;


    private : 
    std::string					        _realName;
    std::string          		        _nickName;
    std::string                         _username;
    std::string                         _buffer;
	//std::map<std::string, Channel*>     _channels;
    int                                 _sd;
    bool                                _isRegistered;
    bool                                _isNickNameSet;
    bool                                _isRealNameSet;
    std::string                         _pass;
    bool                                _passOK;;
};



