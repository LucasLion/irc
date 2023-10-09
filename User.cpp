
#include "Server.hpp"
#include "User.hpp"
#include "Command.hpp"
#include <string.h>

User::User() { }

User::User(int num) : _num(num) { }

void    User::addBuffer( char *Buf ) {
    _buffer.assign(Buf, strlen(Buf));
}

void	User::parseBuffer(){

	size_t	                start = 0;
    size_t	                crlfPos;
    Command                 cmd;

    while ((crlfPos = _buffer.find("\r\n", start)) != std::string::npos) {
        cmd.raw_message = (_buffer.substr(start, crlfPos - start));
		cmd.parseInput();
        messages.push_back(cmd);
        messagesString.push_back(_buffer.substr(start, crlfPos - start));
        start = crlfPos + 2;
    }

    if (start < _buffer.length()) {
        _buffer = _buffer.substr(start);
    }
}


void User::printMessages() {
    std::vector<std::string>::const_iterator it;
    
    for (it = messagesString.begin(); it != messagesString.end(); ++it) {
        std::cout << *it << "\n";
        if (it != messagesString.end() - 1) {
            std::cout << "\n";
        }
    }
}

std::string User::cap( std::string str ) {
	std::cout << "On parse à cet endroit" << std::endl;

	if (str == "CAP")
		std::cout << "Exemple: str = 'CAP'" << std::endl;
	return str;
}
