#pragma once

#include "Server.hpp"
#include <vector>
#include <string>

class Command{
    
    public :

    void                        parseIpnut();
    char*                       generateResponse();
    void                        printCommand();
    std::string                 raw_message;

    private : 
    std::string                 tag;
    std::string                 source;
    std::string                 command;
    std::vector<std::string>    param; 
};
