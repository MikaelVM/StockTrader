#include "../include/library.h"
#include "doctest.h"
#include <iostream>

void hello() {
    std::cout << "Hello, World!" << std::endl;
}

TEST_CASE("Does test work")
{
    auto a = 1;

    SUBCASE("Test"){
        CHECK(a == 1);
    }
}