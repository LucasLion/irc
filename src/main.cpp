
#include "../includes/header.hpp"
#include "../includes/Server.hpp"



int main( int argc , char *argv[] ) {
	try {
		if (argc == 3) {
			Server	server(argv[1], argv[2]);
			server.run();
		}
		else 
			throw std::runtime_error(ERROR("Usage: ./server <port> <password>"));
	} catch (std::exception &e) {
		std::cout << ERROR("Error: ") << e.what() << std::endl;
	}
	return (0);
}
