#pragma once

#include <memory>

// Forward declarations for modular components
class DataFeed;
class FeatureEngine;
class StrategyEngine;
class TradeExecutor;

class Orchestrator {
public:
    Orchestrator();
    ~Orchestrator();

    // Boot entry point
    void run();

private:
    // Core steps in boot and strategy execution
    void load_config();
    void start_data_pipeline();
    void run_strategy_loop();
    void shutdown();

    bool is_running;

    // Injected components (will be assigned in .cpp constructor or later)
    std::unique_ptr<DataFeed> data_feed;
    std::unique_ptr<FeatureEngine> feature_engine;
    std::unique_ptr<StrategyEngine> strategy_engine;
    std::unique_ptr<TradeExecutor> trade_executor;
};
