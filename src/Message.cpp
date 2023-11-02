
#include "../includes/header.hpp"
#include "../includes/Message.hpp"

void	Message::parseArgs( void ) { return; }

void	Message::parseParam( std::string params ) {

	size_t	                start = 0;
    size_t	                esp;

    while ((esp = params.find(" ", start)) != std::string::npos) {
		if (params[start] == ':') {
			_param.push_back(params.substr(start + 1));
			return;
		}
        std::string param;
        param = (params.substr(start, esp - start));
        _param.push_back(param); 
        start = esp + 1;
    }
	if (params[start] == ':' && params.length() > 1)
		start++;
    std::string para = (params.substr(start, esp - start));
    _param.push_back(para); 
}

void	Message::parseInput( void ) {

    size_t spacePos = rawMessage.find(' ');

    if (spacePos == std::string::npos)
		return;

    std::string Part = rawMessage.substr(0, spacePos);
	if (Part[0] == '@')
		_tag = Part.substr(1); 
	else
		spacePos = -1; 

    size_t spacePos2 = rawMessage.find (' ', spacePos + 1);
	if (spacePos2 == std::string::npos)
        return;

	Part = rawMessage.substr(spacePos + 1, spacePos2 - spacePos - 1);
	if (Part[0] == ':')
        _source = Part.substr(1); 
	else 
		spacePos2 = spacePos;

    size_t spacePos3 = rawMessage.find(' ', spacePos2 + 1);
	if (spacePos3 == std::string::npos)
        _command = rawMessage.substr(spacePos2 + 1);
	else {
        _command = rawMessage.substr(spacePos2 + 1, spacePos3 - spacePos2 - 1);
        _paramstrng = rawMessage.substr(spacePos3 + 1);
		parseParam(rawMessage.substr(spacePos3 + 1));
    }
}

void    Message::printCommand( void ) {

	std::vector<std::string>::iterator it;
    
	std::cout << "-------------" << std::endl;
    std::cout << "rawMessage : " <<  rawMessage << std::endl;
    std::cout << "TAG         : " <<  _tag << std::endl;
    std::cout << "SOURCE      : " <<  _source << std::endl;
    std::cout << "COMMAND     : " <<  _command << std::endl;
    std::cout << "PARAMSTRING : " << _paramstrng <<std::endl;
	std::cout << "PARAMS      : " ;

    for (it = _param.begin(); it != _param.end(); ++it) {
        std::cout << *it << " --- ";
	}
	std::cout << std::endl << "-------------" << std::endl;
}

std::string Message::getCommand( void ) { return _command; }
std::string Message::getSource( void ) { return _source; }
std::string Message::getTag( void ) { return _tag; }
std::string Message::getParam( int i ) { return _param[i]; }
int         Message::nbParam( void ) { return _param.size(); }
