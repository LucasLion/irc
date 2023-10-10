
#include "User.hpp"
#include "Command.hpp"
#include <locale>

User::User( void ) { }

User::User( int num ) : _num(num) { }

void    User::addBuffer( char *buf ){
    _buffer.assign(buf, strlen(buf));
}

void	User::parseBuffer( void ) {

	size_t	                start = 0;
    size_t	                crlfPos;
    Command                 cmd;

    while ((crlfPos = _buffer.find("\n", start)) != std::string::npos) {
        cmd.rawMessage = (_buffer.substr(start, crlfPos - start));
		//cmd.parseInput();
        messages.push_back(cmd);
        messagesString.push_back(_buffer.substr(start, crlfPos - start));
        start = crlfPos + 2;
    }

    if (start < _buffer.length()) {
        _buffer = _buffer.substr(start);
    }
}


void User::printMessages( void ) {
    std::vector<std::string>::const_iterator it;
    
    for (it = messagesString.begin(); it != messagesString.end(); ++it) {
        std::cout << *it << "\n";
        if (it != messagesString.end() - 1) {
            std::cout << "\n";
        }
    }
}

void User::printCommands( void ) {
    std::vector<Command>::iterator it;
    
    for (it = messages.begin(); it != messages.end(); ++it) {
		it->parseInput();
    }
	if (messages.size() > 0)
		messages.pop_back();
}


std::string User::cap( std::string str ) {
	std::cout << "On parse à cet endroit" << std::endl;

	if (str == "CAP")
		std::cout << "Exemple: str = 'CAP'" << std::endl;
	return str;
}
