#include <catch2/catch.hpp>
#include "utils/logger.hpp"

TEST_CASE("Logger instance creation", "[logger]")
{
    auto logger = Logger::get_instance();
    REQUIRE(logger != nullptr);
    logger->info("Logger test message");
}
