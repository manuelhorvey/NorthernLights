#pragma once

#include <map>
#include <memory>
#include <optional>
#include <string>
#include <toml++/toml.h>

#include "data/market_data.hpp"

class RegimeSwitcher
{
   public:
    struct RegimeProfile
    {
        std::string name;
        double      volatility_threshold = 0.0;
        double      volume_threshold     = 0.0;
        double      slippage_buffer      = 0.0;
        std::string entry_aggression     = "moderate";
    };

    // Existing constructor
    RegimeSwitcher(MarketData* market_data_ptr, const std::string& symbol_key);

    // New overload: construct with only MarketData*
    RegimeSwitcher(MarketData* market_data_ptr);

    bool                       load_profiles(const std::string& filepath);
    std::optional<std::string> detect(double volatility, double volume) const;
    void                       print_profiles() const;

    void                       update(const std::string& symbol);  // Called in orchestrator
    std::optional<std::string> current() const
    {
        return current_regime;
    }

    // Setter to set/change symbol after construction
    void set_symbol(const std::string& symbol_key)
    {
        symbol = symbol_key;
    }

   private:
    std::map<std::string, RegimeProfile> regimes;
    std::optional<std::string>           current_regime;

    MarketData* market_data;  // Not owned
    std::string symbol;       // Symbol for which regime is being computed

    std::optional<RegimeProfile> parse_profile(const std::string& name, const toml::table& t);

    // Compute volatility & volume from recent market data
    double compute_volatility() const;
    double compute_recent_volume() const;
};
