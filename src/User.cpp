
#include "../includes/header.hpp"
#include "../includes/User.hpp"
#include "../includes/Message.hpp"

User::User( void ) { 
    setRegistered(false);
    setNickNameSet(false);
    setRealNameSet(false);
    _passOK = false;
}

void User::printCommands( void ) {
        for (std::vector<Message>::iterator it = messages.begin(); it != messages.end();) {
        it->printCommand();
        it = messages.erase(it);
    }
}

void	User::getBuffer( char *buf ) {

	size_t	                start = 0;
    size_t	                crlfPos;
    size_t                  lfPos;

        _buffer.append(buf, strlen(buf));
       while ((crlfPos = _buffer.find("\r\n", start)) != std::string::npos ||
           (lfPos = _buffer.find("\n", start)) != std::string::npos) {

        size_t pos;
        if (crlfPos != std::string::npos && lfPos != std::string::npos) {
            pos = std::min(crlfPos, lfPos);
        } else if (crlfPos != std::string::npos) {
            pos = crlfPos;
        } else {
            pos = lfPos;
        }

        Message cmd;
        cmd.rawMessage = (_buffer.substr(start, pos - start));
		cmd.parseInput();
        messages.push_back(cmd); 
        start = pos + 2;
    }
     if (start > 0) {
        _buffer = _buffer.substr(start);
    }
}



void	User::addChannel( std::string chanName, Channel* channel ) {
    _channels[chanName] = channel;
}

void	User::setRealName( std::string realName ) {
	_realName = realName;
}

void	User::setNickName( std::string nickName ) {
	_nickName = nickName;
}

void	User::setSd( int sd ) {
	_sd = sd;
}

std::string	User::getRealName( void ) {
	return _realName;
}

std::string	User::getNickName( void ) {
	return _nickName;
}

std::map<std::string, Channel*>	User::getChannels( void ) {
	return _channels;
}

int User::getSd( void ){
    return _sd;
}

bool User::isRegistered(){
    return _isRegistered;
}

void User::setRegistered(bool isRegistered){
    _isRegistered = isRegistered;
}

bool User::isNickNameSet(){
    return _isNickNameSet;
}

void User::setNickNameSet(bool isNickNameSet){
    _isNickNameSet = isNickNameSet;
}

bool User::isRealNameSet(){
    return _isRealNameSet;
}

void User::setRealNameSet(bool isRealNameSet){
    _isRealNameSet = isRealNameSet;
}

void User::setUserName(std::string userName){
    _username = userName;
}

std::string User::getUserName(){
    return _username;
}

std::string User::getPass(){
    return _pass;
}

void User::setPass(std::string pass){
    _pass = pass;
}

void User::setPassOK(bool passOK){
    _passOK = passOK;
}

bool User::isPassOK(){
    return _passOK;
}