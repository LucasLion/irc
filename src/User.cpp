
#include "../includes/header.hpp"
#include "../includes/User.hpp"
#include "../includes/Message.hpp"

User::User( void ) { 
    setRegistered(false);
    setNickNameSet(false);
    setRealNameSet(false);
    _passOk = false;
}

void	User::getBuffer( char *buf,  ssize_t bytesRead ) {

	size_t	                start = 0;
    size_t	                crlfPos;
    size_t                  lfPos;

	if (bytesRead <= 0) {
        return;
    }
	_buffer.append(buf, bytesRead);

	while ((crlfPos = _buffer.find("\r\n", start)) != std::string::npos || (lfPos = _buffer.find("\n", start)) != std::string::npos) {
		size_t pos;
		if (crlfPos != std::string::npos && lfPos != std::string::npos)
			pos = std::min(crlfPos, lfPos);
		else if (crlfPos != std::string::npos)
			pos = crlfPos;
		else
			pos = lfPos;
		if (start < _buffer.size()) {
			Message cmd;
			cmd.rawMessage = (_buffer.substr(start, pos - start));
			cmd.parseInput();
			messages.push_back(cmd); 
			start = pos + 2;
		} else 
			break;
	}
	if (start >= 0 && start <= _buffer.size())
        _buffer = _buffer.substr(start);
    else 
   		_buffer.clear();
}


void							User::addChannel( std::string chanName, Channel* channel ) { _channels[chanName] = channel; }
void							User::removeChannel( std::string chanName ) { _channels.erase(chanName); }
void							User::setRealName( std::string realName ) { _realName = realName; }
void							User::setNickName( std::string nickName ) { _nickName = nickName; }
void							User::setSd( int sd ) { _sd = sd; }
std::string						User::getRealName( void ) { return _realName; }
std::string						User::getNickName( void ) { return _nickName; }
std::map<std::string, Channel*>	User::getChannels( void ) { return _channels; }
int								User::getSd( void ) { return _sd; }
bool							User::isRegistered( void ) { return _isRegistered; }
void 							User::setRegistered( bool isRegistered ){ _isRegistered = isRegistered; }
bool 							User::isNickNameSet( void ) { return _isNickNameSet; }
void 							User::setNickNameSet( bool isNickNameSet ) { _isNickNameSet = isNickNameSet; }
bool 							User::isRealNameSet( void ) { return _isRealNameSet; }
void 							User::setRealNameSet( bool isRealNameSet ) { _isRealNameSet = isRealNameSet; }
void 							User::setUserName( std::string userName ) { _username = userName; }
std::string						User::getUserName( void ) { return _username; }
std::string 					User::getPass( void ) { return _pass; }
void							User::setPass( std::string pass ) { _pass = pass; }
void 							User::setPassOK( bool passOk ) { _passOk = passOk; }
bool 							User::isPassOK( void ) { return _passOk; }
