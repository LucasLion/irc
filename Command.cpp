
#include "Command.hpp"
#include "Server.hpp"

//static void printVector( std::vector<std::string> commands ) {
//	std::vector<std::string>::iterator it;
//
//	for (it = commands.begin(); it != commands.end(); it++) {
//		std::cout << "Command: " << *it << std::endl;
//	}
//}

//@id=234AB :dan!d@localhost PRIVMSG #chan :Hey what's up!

void	Command::parseArgs( void ) {
	
	for (int i = 0; i < static_cast<int>(_param.size()); i++) {
		std::cout << "Params: " << _param[i] << std::endl;
	}
}

void	Command::parseInput( void ) {

	std::vector<std::string> commands;
	std::istringstream	iss(rawMessage);
	std::string			command;
	int					commandPos;
	
	while (iss >> command) {
		commands.push_back(command);
	}

	if (!commands.empty()) {
		if (commands[0][0] == '@') {
			_tag = commands[0];
			if (commands[1][0] == ':') {
				_source = commands[1];
				_command = commands[2];
				commandPos = 2;
			}
			else {
				_command = commands[1];
				commandPos = 1;
			}
		}
		else if (commands[0][0] == ':') {
			_source = commands[0];
			_command = commands[1];
			commandPos = 1;
		}
		else {
			_command = commands[0];
			commandPos = 0;
		}

		for (int i = 1; i < static_cast<int>(commands.size()) - commandPos; i++) {
			_param.push_back(commands[commandPos + i]);
		}
		parseArgs();
		printCommand();
	}
}

void    Command::printCommand( void ) {

    std::vector<std::string>::iterator it;
    
    std::cout << "raw_message : " <<  rawMessage << std::endl;
    std::cout << "TAG         : " <<  _tag << std::endl;
    std::cout << "SOURCE      : " <<  _source << std::endl;
    std::cout << "COMMAND     : " <<  _command << std::endl;
    
    for (it = _param.begin(); it != _param.end(); ++it) {
        std::cout << "PARAM       : " << *it << std::endl;   
    }
}
