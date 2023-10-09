
#pragma once

#include "Server.hpp"
#include <vector>
#include <string>


struct message{
    std::string                 raw_message;
    std::string                 tag;
    std::string                 source;
    std::string                 command;
    std::vector<std::string>    param; 
};

class Parser{

	public :

						Parser();
						Parser(char *raw_input);

	std::string					raw_input;
	std::vector<std::string>	messages;
	void						parseInput();
	int							generateResponse();
	std::string 				cap( std::string command );
	void						printMessages();
	
};


