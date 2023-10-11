
#include "Server.hpp"

//@id=234AB :dan!d@localhost PRIVMSG #chan :Hey what's up!

void	Command::parseInput( void ) {

	std::vector<std::string> commands;
	std::string			word;
	int					commandPos;
	std::string			tmp;


	tmp = rawMessage;
	size_t position = 0;

	//while ((position = tmp.find(" ")) != std::string::npos) {
	while ((position = tmp.find(" ")) < tmp.size()) {
		word = tmp.substr(0, position);
		commands.push_back(word);
		tmp.erase(0, position + 1);
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
			std::cout << "commandPos: " << commands[commandPos + i] << std::endl;
			_param.push_back(commands[commandPos + i]);

		}
	}
}

void    Command::printCommand( void ) {

    std::vector<std::string>::iterator it;
    
    std::cout << "rawMessage : " <<  rawMessage << std::endl;
    std::cout << "TAG         : " <<  _tag << std::endl;
    std::cout << "SOURCE      : " <<  _source << std::endl;
    std::cout << "COMMAND     : " <<  _command << std::endl;
    
	int i = 1;
    for (it = _param.begin(); it != _param.end(); ++it) {
        std::cout << "PARAM " << i << "     : " << *it << std::endl;   
		i++;
    }
}
