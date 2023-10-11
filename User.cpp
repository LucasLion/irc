
#include "User.hpp"
#include "Command.hpp"
#include <locale>

User::User( void ) { }

//User::User( int num ) : _num(num) { }

void	User::getBuffer( char *buf ) {

	size_t	                start = 0;
    size_t	                crlfPos;


    _buffer.assign(buf, strlen(buf));

    while ((crlfPos = _buffer.find("\n", start)) != std::string::npos) {
        Command cmd;
        cmd.rawMessage = (_buffer.substr(start, crlfPos - start));
		cmd.parseInput();
        _messages.push_back(cmd); 
        start = crlfPos + 2;
    }
    if (start < _buffer.length()) {
        _buffer = _buffer.substr(start);
    }
}

void User::printCommands( void ) {
        for (std::vector<Command>::iterator it = _messages.begin(); it != _messages.end();) {
        it->printCommand();
        it = _messages.erase(it);
    }
}

void User::generateResponse( int sd ) {
        for (std::vector<Command>::iterator it = _messages.begin(); it != _messages.end();) {
        it->generateResponse( sd );
        it = _messages.erase(it);
    }
}
