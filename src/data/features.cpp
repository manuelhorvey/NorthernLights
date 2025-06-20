#include "../include/data/features.hpp"
#include <cmath>
#include <iostream>

FeatureEngine::FeatureEngine() = default;

void FeatureEngine::set_market_data(MarketData* md)
{
    market_data_ = md;
}

void FeatureEngine::set_symbol(const std::string& symbol)
{
    symbol_ = symbol;
}

void FeatureEngine::compute()
{
    if (!market_data_)
    {
        std::cerr << " FeatureEngine: MarketData not injected.\n";
        return;
    }

    const auto& data = market_data_->get_all(symbol_);
    std::cout << " FeatureEngine: Computing features on " << data.size() << " bars...\n";

    constexpr size_t period = 14;

    if (auto val = sma(data, period); val)
        std::cout << " SMA(" << period << "): " << *val << "\n";
    else
        std::cout << " Not enough data for SMA(" << period << ")\n";

    if (auto val = rsi(data, period); val)
        std::cout << " RSI(" << period << "): " << *val << "\n";
    else
        std::cout << " Not enough data for RSI(" << period << ")\n";

    if (auto val = vwap(data, period); val)
        std::cout << " VWAP(" << period << "): " << *val << "\n";
    else
        std::cout << " Not enough data for VWAP(" << period << ")\n";

    if (auto val = atr(data, period); val)
        std::cout << " ATR(" << period << "): " << *val << "\n";
    else
        std::cout << " Not enough data for ATR(" << period << ")\n";
}

// === Static indicators ===

std::optional<double> FeatureEngine::sma(const std::vector<OHLCV>& data, size_t period)
{
    if (data.size() < period)
        return std::nullopt;

    double sum = 0.0;
    for (size_t i = data.size() - period; i < data.size(); ++i) sum += data[i].close;

    return sum / period;
}

std::optional<double> FeatureEngine::rsi(const std::vector<OHLCV>& data, size_t period)
{
    if (data.size() < period + 1)
        return std::nullopt;

    double gain = 0.0, loss = 0.0;
    for (size_t i = data.size() - period; i < data.size(); ++i)
    {
        double delta = data[i].close - data[i - 1].close;
        if (delta >= 0)
            gain += delta;
        else
            loss -= delta;
    }

    if (loss == 0)
        return 100.0;
    double rs = gain / loss;
    return 100.0 - (100.0 / (1.0 + rs));
}

std::optional<double> FeatureEngine::vwap(const std::vector<OHLCV>& data, size_t period)
{
    if (data.size() < period)
        return std::nullopt;

    double pv = 0.0, vol = 0.0;
    for (size_t i = data.size() - period; i < data.size(); ++i)
    {
        const auto& d       = data[i];
        double      typical = (d.high + d.low + d.close) / 3.0;
        pv += typical * d.volume;
        vol += d.volume;
    }

    return vol > 0.0 ? std::optional<double>(pv / vol) : std::nullopt;
}

std::optional<double> FeatureEngine::atr(const std::vector<OHLCV>& data, size_t period)
{
    if (data.size() < period + 1)
        return std::nullopt;

    double sum = 0.0;
    for (size_t i = data.size() - period; i < data.size(); ++i)
    {
        double high       = data[i].high;
        double low        = data[i].low;
        double prev_close = data[i - 1].close;

        double tr =
            std::max(high - low, std::max(std::abs(high - prev_close), std::abs(low - prev_close)));

        sum += tr;
    }

    return sum / period;
}
