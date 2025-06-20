#include <catch2/catch.hpp>
#include "data/market_data.hpp"

TEST_CASE("MarketData add and retrieve ticks per symbol", "[marketdata]") {
    MarketData md;

    std::string symbol = "XAUUSD";
    OHLCV tick{
        std::chrono::system_clock::now(),
        104658.59, 104820.00, 104391.38,
        104596.63, 1621
    };

    md.add_tick(symbol, tick);

    auto latest = md.get_latest(symbol);
    REQUIRE(latest.has_value());
    REQUIRE(latest->close == Approx(104596.63));

    double avg_close = md.average_close(symbol, 1);
    REQUIRE(avg_close == Approx(104596.63));
}
