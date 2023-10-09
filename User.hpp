
#pragma once

#include <vector>
#include <string>
#include "Command.hpp"

class User{

    public : 

                                User();
                                User(int num);
    void                        addBuffer(char * buf);
    void                        parseBuffer();
    int							generateResponse();
    void						printMessages();

    private : 
    std::string                 _realName;
    std::string                 _nickName;
    std::string                 _buffer;
    std::vector<Command>	    messages;
	std::vector<std::string>	messagesString;
	std::string 				cap( std::string command );
    int                         _num;
	

};
