

#include "Server.hpp"
#include "Parser.hpp"

int main(int argc , char *argv[])
{
	static_cast<void>(argc);
	Server	server(argv[1]);
	
	//server.setPort(argv[1]);
	server.loop();
	
	
	return 0;
}

