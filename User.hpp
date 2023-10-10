
#pragma once

#include <vector>
#include <string>
#include "Command.hpp"
#include <iostream>
#include <string>
#include <strings.h>
#include <string.h>

class User{

    public : 

                                User();
                                User(int num);
    void                        addBuffer(char * buf);
    void                        parseBuffer( void );
    int							generateResponse( void );
    void						printMessages( void );
//    void                        printCommands( void );

    private : 
    std::string                 _realName;
    std::string                 _nickName;
    std::string                 _buffer;
    std::vector<Command>	    messages;
	std::vector<std::string>	messagesString;
	std::string 				cap( std::string command );
    int                         _num;
	

};
