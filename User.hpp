
#pragma once

#include "header.hpp"
#include "Command.hpp"

class User{

    public : 

                                User();
                                User(int num);
    void                        getBuffer( char * buf );
    int							generateResponse( void );
    void                        printCommands( void );

    private : 
    std::string                 _realName;
    std::string                 _nickName;
    std::string                 _buffer;
    std::vector<Command>	    _messages;
    int                         _num;
	

};
