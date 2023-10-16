
#include "../includes/header.hpp"
#include "../includes/User.hpp"
#include "../includes/Message.hpp"

User::User( void ) { }

//User::User( int num ) : _num(num) { }

void User::printCommands( void ) {
        for (std::vector<Message>::iterator it = messages.begin(); it != messages.end();) {
        it->printCommand();
        it = messages.erase(it);
    }
}

//void User::generateResponse( int sd ) {
//	for (std::vector<Message>::iterator it = messages.begin(); it != messages.end();) {
//		it->generateResponse( sd );
//		it = messages.erase(it);
//    }
//}



void	User::getBuffer( char *buf ) {

	size_t	                start = 0;
    size_t	                crlfPos;

    _buffer.assign(buf, strlen(buf));

    while ((crlfPos = _buffer.find("\r\n", start)) != std::string::npos) {
        Message cmd;
        cmd.rawMessage = (_buffer.substr(start, crlfPos - start));
		cmd.parseInput();
        messages.push_back(cmd); 
        start = crlfPos + 2;
    }
}

void	User::setRealName( std::string realName ) {
	_realName = realName;
}

void	User::setNickName( std::string nickName ) {
	_nickName = nickName;
}

std::string	User::getRealName( void ) {
	return _realName;
}

std::string	User::getNickName( void ) {
	return _nickName;
}
