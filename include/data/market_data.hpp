#pragma once

#include <chrono>
#include <vector>
#include <optional>
#include <string>
#include <unordered_map>

struct OHLCV {
    std::chrono::system_clock::time_point timestamp;
    double open;
    double high;
    double low;
    double close;
    double volume;
};

class MarketData {
public:
    // Lifecycle
    void start();    
    void stop();     

    // Config-based loading
    void load_from_config(const std::string& router_path);
    void load_from_csv(const std::string& symbol, const std::string& filepath);

    // Tick ingestion & retrieval
    void add_tick(const std::string& symbol, const OHLCV& tick);
    std::optional<OHLCV> get_latest(const std::string& symbol) const;
    const std::vector<OHLCV>& get_all(const std::string& symbol) const;

    // Analytics
    double average_close(const std::string& symbol, size_t last_n) const;

    // NEW: Return all loaded symbols
    std::vector<std::string> get_loaded_symbols() const;

private:
    std::unordered_map<std::string, std::vector<OHLCV>> symbol_ticks;
};
