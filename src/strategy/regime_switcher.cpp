#include "../include/strategy/regime_switcher.hpp"
#include "../include/utils/logger.hpp"

#include <toml++/toml.h>
#include <iostream>
#include <numeric>
#include <cmath>

// New constructor overload without symbol key
RegimeSwitcher::RegimeSwitcher(MarketData* market_data_ptr)
    : market_data(market_data_ptr), symbol("") {}

// Existing constructor
RegimeSwitcher::RegimeSwitcher(MarketData* market_data_ptr, const std::string& symbol_key)
    : market_data(market_data_ptr), symbol(symbol_key) {}

bool RegimeSwitcher::load_profiles(const std::string& filepath) {
    try {
        auto table = toml::parse_file(filepath);

        for (const auto& [key, val] : table) {
            if (!val.is_table()) continue;

            const auto& t = *val.as_table();
            std::string regime_name = std::string(key.str());

            auto profile_opt = parse_profile(regime_name, t);
            if (profile_opt.has_value()) {
                regimes[regime_name] = profile_opt.value();
            } else {
                Logger::get_instance()->warn("⚠️ Skipping invalid regime '{}'", regime_name);
            }
        }

        Logger::get_instance()->info("✅ Loaded {} regime profiles from {}", regimes.size(), filepath);
        return true;

    } catch (const toml::parse_error& err) {
        Logger::get_instance()->error("❌ Failed to parse regime file '{}': {}", filepath, err.description());
        return false;
    }
}

std::optional<RegimeSwitcher::RegimeProfile> RegimeSwitcher::parse_profile(const std::string& regime_key, const toml::table& t) {
    RegimeProfile profile;

    if (auto n = t.get_as<std::string>("name"))
        profile.name = std::string(*n);
    else {
        Logger::get_instance()->warn("Missing 'name' for '{}'", regime_key);
        return std::nullopt;
    }

    if (auto v = t["volatility_threshold"].as_floating_point())
        profile.volatility_threshold = v->get();
    else
        Logger::get_instance()->warn("Missing 'volatility_threshold' for '{}', defaulting to 0.0", regime_key);

    if (auto v = t["volume_threshold"].as_integer())
        profile.volume_threshold = static_cast<double>(v->get());
    else
        Logger::get_instance()->warn("Missing 'volume_threshold' for '{}', defaulting to 0.0", regime_key);

    if (auto s = t["slippage_buffer"].as_floating_point())
        profile.slippage_buffer = s->get();
    else
        Logger::get_instance()->warn("Missing 'slippage_buffer' for '{}', defaulting to 0.0", regime_key);

    if (auto e = t["entry_aggression"].as_string())
        profile.entry_aggression = e->get();
    else
        Logger::get_instance()->warn("Missing 'entry_aggression' for '{}', defaulting to 'moderate'", regime_key);

    return profile;
}

std::optional<std::string> RegimeSwitcher::detect(double volatility, double volume) const {
    for (const auto& [key, profile] : regimes) {
        if (volatility >= profile.volatility_threshold &&
            volume >= profile.volume_threshold) {
            return key;
        }
    }
    return std::nullopt;
}

void RegimeSwitcher::update(const std::string& symbol_key) {
    symbol = symbol_key;  // Update the internal symbol key before computation

    double volatility = compute_volatility();
    double volume = compute_recent_volume();

    auto detected = detect(volatility, volume);
    if (detected && current_regime != detected) {
        current_regime = detected;
        Logger::get_instance()->info("🔁 Regime switched to: {}", current_regime.value());
    } else if (!detected) {
        Logger::get_instance()->warn("⚠️ No matching regime found for volatility={} volume={}", volatility, volume);
    }
}

double RegimeSwitcher::compute_volatility() const {
    const auto& data = market_data->get_all(symbol);
    if (data.size() < 2) return 0.0;

    std::vector<double> returns;
    for (size_t i = 1; i < data.size(); ++i) {
        if (data[i - 1].close <= 0.0) continue;
        double ret = std::log(data[i].close / data[i - 1].close);
        returns.push_back(ret);
    }

    if (returns.size() < 2) return 0.0;

    double mean = std::accumulate(returns.begin(), returns.end(), 0.0) / returns.size();
    double variance = 0.0;

    for (double r : returns)
        variance += std::pow(r - mean, 2);

    return std::sqrt(variance / (returns.size() - 1));
}

double RegimeSwitcher::compute_recent_volume() const {
    const auto& data = market_data->get_all(symbol);
    if (data.empty()) return 0.0;

    size_t count = std::min<size_t>(data.size(), 10);
    double total_volume = 0.0;

    for (size_t i = data.size() - count; i < data.size(); ++i) {
        if (data[i].volume > 0.0)
            total_volume += data[i].volume;
    }

    return total_volume;
}

void RegimeSwitcher::print_profiles() const {
    for (const auto& [key, profile] : regimes) {
        std::cout << "Regime: " << key << "\n"
                  << "  Name: " << profile.name << "\n"
                  << "  Volatility Threshold: " << profile.volatility_threshold << "\n"
                  << "  Volume Threshold: " << profile.volume_threshold << "\n"
                  << "  Slippage Buffer: " << profile.slippage_buffer << "\n"
                  << "  Entry Aggression: " << profile.entry_aggression << "\n\n";
    }
}
