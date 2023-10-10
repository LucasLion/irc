#pragma once

#include <vector>
#include <string>
#include <iostream>
#include <string>
#include <strings.h>
#include <string.h>


class Command {
    
	public :

		void                        parseInput( void );
		char*                       generateResponse();
		void                        printCommand();
		std::string                 raw_message;

	private : 

		std::string                 _tag;
		std::string                 _source;
		std::string                 _command;
		std::vector<std::string>    _param; 

};