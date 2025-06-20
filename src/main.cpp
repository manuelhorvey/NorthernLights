#include "infra/orchestrator.hpp"
#include "utils/logger.hpp"

int main(int argc, char* argv[])
{
    auto logger = Logger::get_instance();
    logger->info("🚀 TheNorthernLights trading engine initializing...");

    try
    {
        Orchestrator orchestrator;
        orchestrator.run();
        logger->info("✅ Build & execution completed. Time to make alpha.");
    }
    catch (const std::exception& e)
    {
        logger->error("🔥 Unhandled exception: {}", e.what());
        return EXIT_FAILURE;
    }
    catch (...)
    {
        logger->error("🔥 Unknown fatal error occurred.");
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}
