
#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "doctest/doctest/doctest.h"
#include <thread>

#include "../includes/header.hpp"
#include "../includes/Server.hpp"
#include "../includes/User.hpp"
#include "../includes/Message.hpp"
#include "../includes/Channel.hpp"

int gSignalStatus = 0;

void	handleSignal(int signal) {
	if (signal == SIGINT) {
		gSignalStatus = true;
		std::cout << "signal received: " << signal << std::endl; }
	}

void	runContext(doctest::Context *context) {
	int res = context->run();
	if (context->shouldExit())
		exit(res);
}

Server program(int argc , char *argv[])
{
	Server server(argv[1], argv[2]);
	try {
		if (argc == 3) {
			doctest::Context context;
            //std::thread contextThread([&context]() {
             //   runContext(&context);
            //});
			while (!gSignalStatus) {
				std::this_thread::sleep_for(std::chrono::milliseconds(100));
			}
			//contextThread.join();
			server.run(gSignalStatus);
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

			signal(SIGINT, handleSignal);
	int argc = 3;
	char *argv[] = {(char *)"./ircserv", (char *)"6667", (char *)"test"};

	Server server = program(argc, argv);

	CHECK(server.getPortno() == 6667);
	//CHECK(server.getPortno() == 0);
	//CHECK(server.getPortno() == 0);
}
