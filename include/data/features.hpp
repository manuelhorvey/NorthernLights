#pragma once

#include <optional>
#include <string>
#include <vector>
#include "data/market_data.hpp"

class FeatureEngine
{
   public:
    FeatureEngine();

    void set_market_data(MarketData* md);        // Inject MarketData
    void set_symbol(const std::string& symbol);  // ✅ Set active symbol
    void compute();                              // Compute all active features for symbol

    // Indicators
    static std::optional<double> sma(const std::vector<OHLCV>& data, size_t period);
    static std::optional<double> rsi(const std::vector<OHLCV>& data, size_t period);
    static std::optional<double> vwap(const std::vector<OHLCV>& data, size_t period);
    static std::optional<double> atr(const std::vector<OHLCV>& data, size_t period);

   private:
    MarketData* market_data_ = nullptr;   // Reference, not owned
    std::string symbol_      = "XAUUSD";  // ✅ Default symbol if not set
};
