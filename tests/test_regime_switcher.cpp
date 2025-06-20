#include <catch2/catch.hpp>
#include "strategy/regime_switcher.hpp"
#include "data/market_data.hpp"

TEST_CASE("RegimeSwitcher loads profiles and detects regime", "[regime]") {
    MarketData market_data;
    std::string symbol = "XAUUSD";

    // Add mock OHLCV data for XAUUSD
    market_data.add_tick(symbol, {std::chrono::system_clock::now(), 100, 105, 95, 102, 1'500'000});
    market_data.add_tick(symbol, {std::chrono::system_clock::now(), 102, 108, 101, 107, 1'600'000});
    market_data.add_tick(symbol, {std::chrono::system_clock::now(), 107, 110, 106, 108, 1'550'000});

    // Construct RegimeSwitcher WITHOUT fixed symbol (use new constructor)
    RegimeSwitcher rs(&market_data);

    REQUIRE(rs.load_profiles("../../config/regime_profiles.toml"));

    // Update regime for the symbol explicitly
    rs.update(symbol);

    auto current = rs.current();
    REQUIRE(current.has_value());

    // Manually test detect with known high volatility/volume
    auto regime = rs.detect(0.07, 3'000'000);
    REQUIRE(regime.has_value());
    REQUIRE(*regime == "news_driven");

    rs.print_profiles();
}
