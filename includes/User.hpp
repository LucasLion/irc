
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
    void                            setSd(int sd);
    int                             getSd();
	std::string						getRealName( void );
	std::string						getNickName( void );
	std::map<std::string, Channel>	getChannels( void );
    std::vector<Message>			messages;
    std::string                     ipAdress;


    private : 
    std::string					        _realName;
    std::string          		        _nickName;
    std::string                         _buffer;
	std::map<std::string, Channel>      _channels;
    int                                 _sd;

};



