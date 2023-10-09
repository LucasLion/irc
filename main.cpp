

#include "Server.hpp"
#include "Parser.hpp"

int main(int argc , char *argv[])
{
	if (argc == 3) 
		Server	server(argv[1], argv[2]);
	else 
		std::cout << "wrong number of args" << std::endl;
	return 0;
}

