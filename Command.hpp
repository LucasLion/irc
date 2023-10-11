
#pragma once

#include "header.hpp"

class Command {
    
	public :

	//								Command( void );
	//								Command( std::string raw );
	//								Command( Command const & src );
	//								~Command( void );
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
		const static std::string					_cmd[4];
		

};
