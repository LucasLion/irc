
#define DOCTEST_CONFIG_IMPLEMENT #_WITH_MAIN
#include "doctest/doctest/doctest.h"
#include <thread>

#include "../includes/header.hpp"
#include "../includes/Server.hpp"
#include "../includes/User.hpp"
#include "../includes/Message.hpp"
#include "../includes/Channel.hpp"

volatile sig_atomic_t gSignalStatus = 0;
int g_argc;
char** g_argv;

void	signal_handler(int signal) {
	if (signal == SIGINT) {
		gSignalStatus = signal;
		std::cout << "signal received: " << signal << std::endl;
		//exit(signal);
	}
}

void	runContext(doctest::Context *context) {
	int res = context->run();
	if (context->shouldExit())
		exit(res);
}

int main(int argc , char *argv[])
{
    g_argc = argc;
    g_argv = argv;
	try {
		if (argc == 3) {
			std::cout << "TESTS" << std::endl;
			doctest::Context context;
			context.applyCommandLine(argc, argv);
			Server server(argv[1], argv[2]);

			std::signal(SIGINT, signal_handler);

            std::thread contextThread([&context]() {
                runContext(&context);
            });


			while (!gSignalStatus) {
				std::this_thread::sleep_for(std::chrono::milliseconds(100));
			}
			server.run();

			contextThread.join();
		}
		else 
			throw std::runtime_error(ERROR("Usage: ./server <port> <password>"));
	} catch (std::exception &e) {
		std::cout << ERROR("Error: ") << e.what() << std::endl;
	}
	return 0;
}

//TEST_CASE("testing the initialisation function") {
//
//	doctest::Context context;
//	context.applyCommandLine(g_argc, g_argv);
//	program(g_argc, g_argv);
//	Server server;
//	//CHECK(server.getPortno() == 6667);
//	CHECK(server.getPortno() == 0);
//	CHECK(server.getPortno() == 0);
//}
