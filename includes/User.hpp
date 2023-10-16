
#pragma once

#include "header.hpp"
#include "Message.hpp"

class Server;
class Message;

class User {

    public : 

                                User();
    void                        printCommands( void );
    void                        getBuffer( char * buf );
	void						setRealName( std::string realName );
	void						setNickName( std::string nickName );
	std::string					getRealName( void );
	std::string					getNickName( void );
    std::vector<Message>	    messages;



    private : 
    std::string                 _realName;
    std::string                 _nickName;
    std::string                 _buffer;

};



