
#define DOCTEST_CONFIG_IMPLEMENT #_WITH_MAIN
#include "doctest/doctest/doctest.h"

#include "../includes/header.hpp"
#include "../includes/Server.hpp"
#include "../includes/User.hpp"
#include "../includes/Message.hpp"
#include "../includes/Channel.hpp"


TEST_CASE("testing the initialisation function") {
	Server server;

	CHECK(server.getPortno() == 6667);
}
