#pragma once

#include <memory>

class MarketData;
class FeatureEngine;
class AltData;
class TradeExecutor;
class RegimeSwitcher;

class Orchestrator
{
   public:
    Orchestrator();
    ~Orchestrator();

    void run();  // Entry point

   private:
    void load_config();          // Load strategy, risk, regime configs
    void start_data_pipeline();  // Init market + alt data ingestion
    void run_strategy_loop();    // Main tick-by-tick strategy execution
    void shutdown();             // Cleanup

    bool is_running;

    std::unique_ptr<MarketData>     data_feed;
    std::unique_ptr<FeatureEngine>  feature_engine;
    std::unique_ptr<AltData>        alt_data;
    std::unique_ptr<TradeExecutor>  trade_executor;
    std::unique_ptr<RegimeSwitcher> regime_switcher;
};
