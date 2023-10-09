
#include "Parser.hpp"

	Parser::Parser(){}

	Parser::Parser(char *raw_input) : raw_input(raw_input){


}

void	Parser::parseInput(){

	size_t	start = 0;
    size_t	crlfPos;
	int						i = 0;

    while ((crlfPos = raw_input.find("\r\n", start)) != std::string::npos) {
        messages.push_back(raw_input.substr(start, crlfPos - start));
		i++;
        start = crlfPos + 2;
    }

    if (start < raw_input.length()) {
        messages[i] = raw_input.substr(start);
    }
}


void Parser::printMessages() {
    std::vector<std::string>::const_iterator it;
    
    for (it = messages.begin(); it != messages.end(); ++it) {
        std::cout << *it;
        if (it != messages.end() - 1) {
            std::cout << "\n";
        }
    }
}

std::string Parser::cap( std::string str ) {
	std::cout << "On parse à cet endroit" << std::endl;

	if (str == "CAP")
		std::cout << "Exemple: str = 'CAP'" << std::endl;
	return str;
}
