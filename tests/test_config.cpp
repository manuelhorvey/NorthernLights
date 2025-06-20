#include <catch2/catch.hpp>
#include "utils/config.hpp"

TEST_CASE("ConfigLoader basic load and get", "[config]")
{
    ConfigLoader config;
    bool         loaded = config.load("../../config/strategy.cfg");
    REQUIRE(loaded);

    int some_int = config.get<int>("some_integer_key", 42);
    REQUIRE(some_int == 42);

    config.print();
}
