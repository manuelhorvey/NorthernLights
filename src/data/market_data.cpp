#include "../include/data/market_data.hpp"
#include "../include/utils/logger.hpp"

#include <numeric>
#include <iostream>
#include <fstream>
#include <sstream>
#include <iomanip>
#include <nlohmann/json.hpp>
#include <unordered_map>

using json = nlohmann::json;

bool ends_with(const std::string& str, const std::string& suffix) {
    return str.size() >= suffix.size() &&
           str.compare(str.size() - suffix.size(), suffix.size(), suffix) == 0;
}

std::string trim(const std::string& s) {
    size_t start = s.find_first_not_of(" \t\r\n");
    size_t end = s.find_last_not_of(" \t\r\n");
    return (start == std::string::npos) ? "" : s.substr(start, end - start + 1);
}

bool parse_timestamp(const std::string& ts, std::chrono::system_clock::time_point& out_time) {
    std::tm t = {};
    std::istringstream ss(ts);

    ss >> std::get_time(&t, "%Y-%m-%d %H:%M:%S");
    if (!ss.fail()) {
        out_time = std::chrono::system_clock::from_time_t(std::mktime(&t));
        return true;
    }

    ss.clear(); ss.str(ts);
    ss >> std::get_time(&t, "%Y-%m-%dT%H:%M:%SZ");
    if (!ss.fail()) {
        out_time = std::chrono::system_clock::from_time_t(std::mktime(&t));
        return true;
    }

    ss.clear(); ss.str(ts);
    ss >> std::get_time(&t, "%m/%d/%Y %H:%M");
    if (!ss.fail()) {
        out_time = std::chrono::system_clock::from_time_t(std::mktime(&t));
        return true;
    }

    return false;
}

void MarketData::start() {
    std::cout << " MarketData stream started (mock).\n";
}

void MarketData::stop() {
    std::cout << " MarketData stream stopped.\n";
}

void MarketData::add_tick(const std::string& symbol, const OHLCV& tick) {
    symbol_ticks[symbol].push_back(tick);
}

std::optional<OHLCV> MarketData::get_latest(const std::string& symbol) const {
    auto it = symbol_ticks.find(symbol);
    if (it == symbol_ticks.end() || it->second.empty()) return std::nullopt;
    return it->second.back();
}

const std::vector<OHLCV>& MarketData::get_all(const std::string& symbol) const {
    static const std::vector<OHLCV> empty;
    auto it = symbol_ticks.find(symbol);
    return it != symbol_ticks.end() ? it->second : empty;
}

double MarketData::average_close(const std::string& symbol, size_t last_n) const {
    auto it = symbol_ticks.find(symbol);
    if (it == symbol_ticks.end() || last_n == 0) return 0.0;

    const auto& ticks = it->second;
    size_t start_index = ticks.size() > last_n ? ticks.size() - last_n : 0;

    double sum = 0.0;
    size_t count = 0;
    for (size_t i = start_index; i < ticks.size(); ++i) {
        sum += ticks[i].close;
        ++count;
    }

    return count > 0 ? sum / count : 0.0;
}

void MarketData::load_from_config(const std::string& router_path) {
    std::ifstream file(router_path);
    if (!file.is_open()) {
        Logger::get_instance()->error("❌ Failed to open router config: {}", router_path);
        return;
    }

    json config;
    try {
        file >> config;
    } catch (const json::parse_error& e) {
        Logger::get_instance()->error("❌ JSON parse error in {}: {}", router_path, e.what());
        return;
    }

    if (config.contains("csv_data") && config["csv_data"].contains("symbols")) {
        for (auto& [symbol, path] : config["csv_data"]["symbols"].items()) {
            std::string csv_path = path.get<std::string>();
            Logger::get_instance()->info("📄 Loading OHLCV for {} from {}", symbol, csv_path);
            load_from_csv(symbol, csv_path);
        }
        return;
    }

    Logger::get_instance()->warn("⚠️ No symbols found in CSV config.");
}

std::vector<std::string> MarketData::get_loaded_symbols() const {
    std::vector<std::string> symbols;
    for (const auto& [symbol, _] : symbol_ticks) {
        symbols.push_back(symbol);
    }
    return symbols;
}


void MarketData::load_from_csv(const std::string& symbol, const std::string& filepath) {
    std::ifstream file(filepath);
    if (!file.is_open()) {
        Logger::get_instance()->error(" Failed to open OHLCV CSV file: {}", filepath);
        return;
    }

    std::string line;
    std::getline(file, line);  // Skip header

    size_t skipped = 0, parsed = 0;

    while (std::getline(file, line)) {
        std::istringstream ss(line);
        std::string ts, open, high, low, close, volume;

        std::getline(ss, ts, ',');
        std::getline(ss, open, ',');
        std::getline(ss, high, ',');
        std::getline(ss, low, ',');
        std::getline(ss, close, ',');
        std::getline(ss, volume, ',');

        OHLCV tick;
        if (!parse_timestamp(trim(ts), tick.timestamp)) {
            ++skipped;
            Logger::get_instance()->warn(" Skipping malformed timestamp: '{}'", ts);
            continue;
        }

        try {
            tick.open = std::stod(trim(open));
            tick.high = std::stod(trim(high));
            tick.low = std::stod(trim(low));
            tick.close = std::stod(trim(close));
            tick.volume = std::stod(trim(volume));
        } catch (...) {
            ++skipped;
            Logger::get_instance()->warn(" Skipping malformed row: '{}'", line);
            continue;
        }

        add_tick(symbol, tick);
        ++parsed;
    }

    Logger::get_instance()->info(" Loaded {} ticks for {} ({} skipped)", parsed, symbol, skipped);
}
