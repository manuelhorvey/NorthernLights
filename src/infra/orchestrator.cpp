#include "infra/orchestrator.hpp"
#include "utils/config.hpp"
#include "utils/logger.hpp"

#include "data/alt_data.hpp"
#include "data/features.hpp"
#include "data/market_data.hpp"
#include "execution/trade_executor.hpp"
#include "strategy/regime_switcher.hpp"

#include <chrono>
#include <thread>

Orchestrator::Orchestrator()
    : is_running(false),
      data_feed(std::make_unique<MarketData>()),
      feature_engine(std::make_unique<FeatureEngine>()),
      alt_data(std::make_unique<AltData>()),
      trade_executor(std::make_unique<TradeExecutor>()),
      regime_switcher(std::make_unique<RegimeSwitcher>(data_feed.get()))
{
}

Orchestrator::~Orchestrator() = default;

void Orchestrator::run()
{
    auto logger = Logger::get_instance();
    logger->info(" Orchestrator starting...");

    load_config();
    start_data_pipeline();

    if (trade_executor && data_feed)
    {
        trade_executor->set_market_data(data_feed.get());
    }

    is_running = true;
    run_strategy_loop();

    shutdown();
}

void Orchestrator::load_config()
{
    auto logger = Logger::get_instance();
    logger->info(" Loading configuration files...");

    ConfigLoader config;
    bool         ok_strategy = config.load("../../config/strategy.cfg");
    bool         ok_risk     = config.load("../../config/risk.cfg");

    bool ok_regime = false;
    if (regime_switcher)
    {
        ok_regime = regime_switcher->load_profiles("../../config/regime_profiles.toml");
    }

    if (ok_strategy && ok_risk && ok_regime)
    {
        logger->info(" All configuration files loaded successfully.");
    }
    else
    {
        logger->warn(" One or more configuration files failed to load.");
    }

    config.print();
    if (regime_switcher)
        regime_switcher->print_profiles();
}

void Orchestrator::start_data_pipeline()
{
    auto logger = Logger::get_instance();
    logger->info(" Bootstrapping data ingestion pipeline...");

    if (data_feed)
    {
        data_feed->load_from_config("../../config/routes.json");
        data_feed->start();
    }

    if (feature_engine && data_feed)
    {
        feature_engine->set_market_data(data_feed.get());
    }

    if (alt_data)
    {
        alt_data->load_news();
    }
}

void Orchestrator::run_strategy_loop()
{
    auto logger = Logger::get_instance();
    logger->info(" Entering strategy execution loop...");

    const auto symbols = data_feed->get_loaded_symbols();
    for (int tick = 1; tick <= 5; ++tick)
    {
        logger->info(" Tick {}: evaluating market conditions...", tick);

        for (const auto& symbol : symbols)
        {
            logger->info("🔍 Processing symbol: {}", symbol);

            if (feature_engine)
            {
                feature_engine->set_symbol(symbol);
                feature_engine->compute();
            }

            if (regime_switcher)
            {
                regime_switcher->update(symbol);
            }

            if (trade_executor)
            {
                trade_executor->execute_order(symbol, 10, true);
            }
        }

        std::this_thread::sleep_for(std::chrono::milliseconds(500));
    }
}

void Orchestrator::shutdown()
{
    auto logger = Logger::get_instance();
    logger->info(" Shutting down orchestrator gracefully...");

    if (data_feed)
    {
        data_feed->stop();
    }
}
