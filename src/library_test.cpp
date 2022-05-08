#include "library.h"
#include "doctest.h"


TEST_CASE("Does testing work")
{
    auto a = 1;

    SUBCASE("Doctest Works"){
        CHECK(a == 1);
    }

    SUBCASE("Library Works"){
        CHECK(addition(2,2) == 4);
    }
}