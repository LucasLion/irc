
#pragma once

#include "header.hpp"

class Message {
    
	public :
		void                        parseInput( void );
		void                        parseArgs( void );
		void						generateResponse( int sd );
		void                        printCommand( void );
		void						parseParam( std::string params );
		std::string 				getCommand( void );
		std::string					getParam( int i );
		std::string 				getSource( void );
		std::string 				getTag( void );
		std::string                 rawMessage;
		int 						nbParam( void );

	private : 
		std::string                 				_tag;
		std::string                 				_source;
		std::string                 				_command;
		std::string									_paramstrng;
		std::vector<std::string>				    _param; 
		const static std::string					_cmd[4];
		

};
