
#define DOCTEST_CONFIG_IMPLEMENT #_WITH_MAIN
#include "doctest/doctest/doctest.h"

#include "../includes/header.hpp"
#include "../includes/Server.hpp"

int factorial(int number) { return number > 1 ? factorial(number - 1) * number : 1; }

TEST_CASE("testing the factorial function") {
	CHECK(factorial(0) == 1);  // should fail 
    CHECK(factorial(1) == 1);
    CHECK(factorial(2) == 2);
    CHECK(factorial(3) == 6);
    CHECK(factorial(10) == 3628800);
}
