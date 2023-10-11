
#include "Server.hpp"

//@id=234AB :dan!d@localhost PRIVMSG #chan :Hey what's up!

void	Command::parseArgs( void ) {
	return;	
}

// void	Command::parseInput( void ) {

// 	std::vector<std::string> commands;
// 	std::string			command;
// 	int					commandPos;

// 	size_t position = 0;
// 	while ((position = rawMessage.find(" ")) != std::string::npos) {
// 		command = rawMessage.substr(0, position);
// 		std::cout << "-------> " << command << std::endl;
// 		commands.push_back(command);
// 		rawMessage.erase(0, position);
// 	}
// 	commands.push_back(command);


// 	if (!commands.empty()) {
// 		if (commands[0][0] == '@') {
// 			_tag = commands[0];
// 			if (commands[1][0] == ':') {
// 				_source = commands[1];
// 				_command = commands[2];
// 				commandPos = 2;
// 			}
// 			else {
// 				_command = commands[1];
// 				commandPos = 1;
// 			}
// 		}
// 		else if (commands[0][0] == ':') {
// 			_source = commands[0];
// 			_command = commands[1];
// 			commandPos = 1;
// 		}
// 		else {
// 			_command = commands[0];
// 			commandPos = 0;
// 		}

// 		for (int i = 1; i < static_cast<int>(commands.size()) - commandPos; ++i) {
// 			_param.push_back(commands[commandPos + i]);
// 		}
// 	}
// }

void	Command::parseParam( std::string params ) {

	size_t	                start = 0;
    size_t	                esp;



    while ((esp = params.find(" ", start)) != std::string::npos) {
        std::string param;
        param = (params.substr(start, esp - start));
        _param.push_back(param); 
        start = esp + 1;
    }
    std::string para = (params.substr(start, esp - start));
    _param.push_back(para); 
}


 void	Command::parseInput( void ) {

    size_t spacePos = rawMessage.find(' ');

    if (spacePos == std::string::npos) {
        return;
    }

    std::string Part = rawMessage.substr(0, spacePos);
    if (Part[0] == '@') {
        _tag = Part.substr(1); 
    } else 
		spacePos = -1; 
    size_t spacePos2 = rawMessage.find (' ', spacePos + 1);
    if (spacePos2 == std::string::npos) {
        return;
    }

	Part = rawMessage.substr(spacePos + 1, spacePos2);
    if (Part[0] == ':') {
        _source = Part.substr(1); 
    } else 
	spacePos2 = spacePos;

    size_t spacePos3 = rawMessage.find(' ', spacePos2 + 1);

    if (spacePos3 == std::string::npos) {
        _command = rawMessage.substr(spacePos2 + 1);
    } else {
        _command = rawMessage.substr(spacePos2 + 1, spacePos3 - spacePos2 - 1);
        _paramstrng = rawMessage.substr(spacePos3 + 1);
       parseParam(rawMessage.substr(spacePos3 + 1));
    }
}





void    Command::printCommand( void ) {

    std::vector<std::string>::iterator it;
    
    std::cout << "rawMessage : " <<  rawMessage << std::endl;
    std::cout << "TAG         : " <<  _tag << std::endl;
    std::cout << "SOURCE      : " <<  _source << std::endl;
    std::cout << "COMMAND     : " <<  _command << std::endl;
    std::cout << "PARAMSTRING : " << _paramstrng <<std::endl;
    
    
     std::cout << "PARAMS      : " ;
    for (it = _param.begin(); it != _param.end(); ++it) {
        std::cout << *it << " --- ";
	}
	std::cout  << std::endl;
}





void Command::generateResponse( int sd ){






	send(sd, "CAP * LS :\r\n", 12, 0 );

}
