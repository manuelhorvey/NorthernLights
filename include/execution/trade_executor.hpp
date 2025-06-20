#pragma once

#include <string>
#include <chrono>

class MarketData;  // Forward declaration for dependency injection

class TradeExecutor {
public:
    TradeExecutor(double slippage_percent = 0.05, int latency_ms = 100);

    // Execute a trade order using latest market price
    // Returns executed price after applying slippage
    double execute_order(const std::string& symbol, double quantity, bool is_buy);

    // Setters and getters for slippage and latency
    void set_slippage_percent(double slippage);
    double get_slippage_percent() const;

    void set_latency_ms(int latency);
    int get_latency_ms() const;

    // Set MarketData context
    void set_market_data(MarketData* mktdata);

private:
    double slippage_percent_;   // Slippage as a percentage (e.g., 0.05 = 5%)
    int latency_ms_;            // Simulated network/exchange latency in milliseconds
    MarketData* market_data_;   // Pointer to live market data

    void simulate_latency() const;
    double apply_slippage(double price, bool is_buy) const;
};
