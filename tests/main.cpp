
#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "doctest/doctest/doctest.h"

#include "../includes/header.hpp"
#include "../includes/Server.hpp"
#include "../includes/User.hpp"
#include "../includes/Message.hpp"
#include "../includes/Channel.hpp"

Server program(int argc , char *argv[])
{
	Server server(argv[1], argv[2]);
	try {
		if (argc == 3) {
			doctest::Context context;
			server.run();
			return server;
		}
		else 
			throw std::runtime_error(ERROR("Usage: ./server <port> <password>"));
	} catch (std::exception &e) {
		std::cout << ERROR("Error: ") << e.what() << std::endl;
	}
	return server;
}

TEST_CASE("testing the initialisation function") {

	int argc = 3;
	char *argv[] = {(char *)"./ircserv", (char *)"6667", (char *)"test"};

	Server server = program(argc, argv);
	CHECK(server.getPortno() == 6667);

}
