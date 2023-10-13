
#include "../includes/header.hpp"
#include "../includes/Command.hpp"

void	Command::parseArgs( void ) {
	return;	
}

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

	Part = rawMessage.substr(spacePos + 1, spacePos2 - spacePos - 1);
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

void Command::generateResponse( int sd ) {

	std::cout << "COMMAND_RECEIVED: " << rawMessage << std::endl;
	if (_command == "CAP") {
		send(sd, "CAP * LS\r\n", 12, 0 );
	}
	//if (_command == "NICK") {
	//	std::string protocol = ":localhost 001 utilisateur :Welcome to FT_IRC ";
	//	std::string name = _param[0].append("\r\n");
	//	protocol.append(name);
	//	send(sd, protocol.c_str(), protocol.length(), 0 );
	//}
	if (_command == "USER") {
		send(sd, ":localhost 002 utilisateur :Vos informations d'utilisateur ont été enregistrées avec succes\r\n", 92, 0 );
	}
	if (_command == "PASS") {
		if (_param[0] != "test") {
			send(sd, ":localhost 464 utilisateur :Password incorrect\r\n", 51, 0 );
			throw std::exception();
		}
		else
			send(sd, ":localhost 001 utilisateur :Bienvenue sur le serveur IRC, utilisateur\r\n", 71, 0);
	}
	if (_command == "PING")
		std::cout << SUCCESS("PONG") << std::endl;
	if (_command == "WHOIS") {
		send(sd, ":localhost 318 THE_BEST_NICKNAME :End of /WHOIS list", 51, 0);
	}
}

std::string Command::getCommand( void ) {
	return _command;
}
