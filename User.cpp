
#include "User.hpp"
#include "Command.hpp"
#include <locale>

User::User( void ) { }

User::User( int num ) : _num(num) { }

void	User::getBuffer( char *buf ) {

	size_t	                start = 0;
    size_t	                crlfPos;
    Command                 cmd;

    _buffer.assign(buf, strlen(buf));

    while ((crlfPos = _buffer.find("\n", start)) != std::string::npos) {
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
    std::vector<Command>::iterator it;
    
    for (it = _messages.begin(); it != _messages.end(); ++it) {
		it->printCommand();
    }
	if (_messages.size() > 0)
		_messages.pop_back();
}
