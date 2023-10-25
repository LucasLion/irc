
#include "../includes/header.hpp"
#include "../includes/User.hpp"
#include "../includes/Message.hpp"

User::User( void ) { 
    setRegistered(false);
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

    _buffer.assign(buf, strlen(buf));

    while ((crlfPos = _buffer.find("\r\n", start)) != std::string::npos) {
        Message cmd;
        cmd.rawMessage = (_buffer.substr(start, crlfPos - start));
		cmd.parseInput();
        messages.push_back(cmd); 
        start = crlfPos + 2;
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