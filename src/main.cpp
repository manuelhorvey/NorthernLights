#include <iostream>
#include "../include/utils/logger.hpp"

int main() {
    auto logger = Logger::get_instance();
    logger->info("TheNorthernLights has started 🌌");
    logger->warn("This is just a placeholder strategy runner.");
    logger->info("Build successful. Time to make alpha.");

    return 0;
}
