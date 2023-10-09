#pragma once

#include "Server.hpp"
#include <vector>
#include <string>


class Command{
    
    public :

    void                        parseIpnut( void );
    char*                       generateResponse( void );
    void                        printCommand( void );
    std::string                 raw_message;

    private : 
    std::string                 _tag;
    std::string                 _source;
    std::string                 _command;
    std::vector<std::string>    _param; 
};