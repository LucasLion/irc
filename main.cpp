

#include "Server.hpp"
#include "Parser.hpp"

int main(int argc , char *argv[])
{
	if (argc == 2) 
		Server	server(argv[1]);
	return 0;
}

