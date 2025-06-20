#include "../include/execution/trade_executor.hpp"
#include "../include/utils/logger.hpp"
#include "../include/data/market_data.hpp" // ✅ For OHLCV access

#include <thread>
#include <cmath>
#include <iomanip>
#include <sstream>

TradeExecutor::TradeExecutor(double slippage_percent, int latency_ms)
    : slippage_percent_(slippage_percent), latency_ms_(latency_ms), market_data_(nullptr) {}

void TradeExecutor::set_market_data(MarketData* mktdata) {
    market_data_ = mktdata;
}

void TradeExecutor::set_slippage_percent(double slippage) {
    slippage_percent_ = slippage;
}

double TradeExecutor::get_slippage_percent() const {
    return slippage_percent_;
}

void TradeExecutor::set_latency_ms(int latency) {
    latency_ms_ = latency;
}

int TradeExecutor::get_latency_ms() const {
    return latency_ms_;
}

void TradeExecutor::simulate_latency() const {
    std::this_thread::sleep_for(std::chrono::milliseconds(latency_ms_));
}

double TradeExecutor::apply_slippage(double price, bool is_buy) const {
    double slippage_amount = price * slippage_percent_;
    return is_buy ? price + slippage_amount : price - slippage_amount;
}

double TradeExecutor::execute_order(const std::string& symbol, double quantity, bool is_buy) {
    simulate_latency();

    double raw_price = 0.0;
    std::string latest_ts = "N/A";

    if (market_data_) {
        auto latest = market_data_->get_latest(symbol);  // ✅ FIXED
        if (latest) {
            raw_price = latest->close;
            std::time_t t = std::chrono::system_clock::to_time_t(latest->timestamp);
            std::ostringstream oss;
            oss << std::put_time(std::gmtime(&t), "%Y-%m-%d %H:%M:%S");
            latest_ts = oss.str();
        } else {
            Logger::get_instance()->error("❌ No market data available for symbol '{}' to execute order.", symbol);
            return 0.0;
        }
    } else {
        Logger::get_instance()->error("❌ MarketData pointer is null. Cannot execute order.");
        return 0.0;
    }

    double executed_price = apply_slippage(raw_price, is_buy);

    Logger::get_instance()->info(
        "{} order executed: {} {} @ {:.4f} (raw {:.4f}, slippage {:.2f}%) | 🕒 Market TS: {}",
        is_buy ? "BUY" : "SELL",
        quantity,
        symbol,
        executed_price,
        raw_price,
        slippage_percent_ * 100.0,
        latest_ts
    );

    return executed_price;
}
