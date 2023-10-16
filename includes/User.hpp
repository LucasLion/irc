
#pragma once

#include "header.hpp"
#include "Command.hpp"

class Server;
class Command;

class User {

    public : 

                                User();
    void						generateResponse( int sd );
    void                        printCommands( void );
    void                        getBuffer( char * buf );

    private : 
    std::string                 _realName;
    std::string                 _nickName;
    std::string                 _buffer;
    std::vector<Command>	    _messages;

};



