
#include "../includes/header.hpp"
#include "../includes/User.hpp"
#include "../includes/Command.hpp"

User::User( void ) { }

//User::User( int num ) : _num(num) { }

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
