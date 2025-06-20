#include "infra/orchestrator.hpp"
#include "utils/logger.hpp"

#include <thread>
#include <chrono>

Orchestrator::Orchestrator()
    : is_running(false)
{}

void Orchestrator::run()
{
    auto logger = Logger::get_instance();
    logger->info("🧠 Orchestrator starting...");

    load_config();
    start_data_pipeline();

    is_running = true;
    run_strategy_loop();

    shutdown();
}

void Orchestrator::load_config()
{
    auto logger = Logger::get_instance();
    logger->info("📂 Loading configuration files...");
    
    // TODO: Use ConfigLoader from utils/config to parse:
    // strategy.cfg, risk.cfg, regime_profiles.toml, etc.
}

void Orchestrator::start_data_pipeline()
{
    auto logger = Logger::get_instance();
    logger->info("🔄 Bootstrapping data ingestion...");

    // TODO: Start mocked or live data ingestion:
    // market_data.start()
    // features.compute()
    // alt_data.load_news()
}

void Orchestrator::run_strategy_loop()
{
    auto logger = Logger::get_instance();
    logger->info("Running main strategy loop...");

    // Simulate ticks
    for (int tick = 1; tick <= 5; ++tick) {
        logger->info(" Tick {} → mock processing", tick);

        // TODO: Hook up real feature extraction, signal generation, execution
        std::this_thread::sleep_for(std::chrono::milliseconds(500));
    }
}

void Orchestrator::shutdown()
{
    auto logger = Logger::get_instance();
    logger->info(" Shutting down orchestrator gracefully.");
}
