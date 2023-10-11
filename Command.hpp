
#pragma once

#include "header.hpp"

class Command {
    
	public :

									Command( void );
									Command( std::string raw );
									Command( Command const & src );
									~Command( void );
		void                        parseInput( void );
		void                        parseArgs( void );
		char*                       generateResponse( void );
		void                        printCommand( void );
		std::string                 rawMessage;

	private : 

		std::string                 _tag;
		std::string                 _source;
		std::string                 _command;
		std::vector<std::string>    _param; 

};
