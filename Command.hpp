
#pragma once

#include "header.hpp"

class Command {
    
	public :

		void                        parseInput( void );
		void                        parseArgs( void );
		void						generateResponse( int sd );
		void                        printCommand( void );
		void						parseParam( std::string params );
		std::string                 rawMessage;

	private : 

		std::string                 				_tag;
		std::string                 				_source;
		std::string                 				_command;
		std::string									_paramstrng;
		std::vector<std::string>				    _param; 
		

};
