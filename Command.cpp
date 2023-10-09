
#include "Command.hpp"
#include "Server.hpp"

void	Command::parseInput( void ) {
	std::cout << "Raw message: " << raw_message << std::endl;
}

void    Command::printCommand( void ) {

    std::vector<std::string>::const_iterator it;
    
    std::cout << "raw_message :  " <<  raw_message << "/n";
    std::cout << " TAG ==> " <<  _tag << "/n";
    std::cout << " SOURCE ==> " <<  _source << "/n";
    std::cout << "COMMAND ==> " <<  _command << "/n";
    
    for (it = _param.begin(); it != _param.end(); ++it) {
        std::cout << "PARAM ==> " << *it << "\n";   
    }
}
