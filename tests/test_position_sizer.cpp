// tests/test_position_sizer.cpp
#include <catch2/catch.hpp>
#include "execution/position_sizer.hpp"

TEST_CASE("PositionSizer calculates position size correctly", "[position_sizer]")
{
    PositionSizer sizer(10000.0, 1.0);  // $10,000 balance, 1% risk

    // Risk amount = 1% of 10,000 = 100
    // Position size = 100 / (entry - stop_loss)
    double entry_price     = 50.0;
    double stop_loss_price = 48.0;
    double expected_size   = 100.0 / (50.0 - 48.0);  // 100/2 = 50 units

    double pos_size = sizer.calculate_position_size(entry_price, stop_loss_price);
    REQUIRE(pos_size == Approx(expected_size));

    // Test with zero stop loss difference -> should return 0 (no risk)
    pos_size = sizer.calculate_position_size(50.0, 50.0);
    REQUIRE(pos_size == 0.0);
}

TEST_CASE("PositionSizer getters and setters work", "[position_sizer]")
{
    PositionSizer sizer(10000.0, 1.0);

    sizer.set_account_balance(20000.0);
    REQUIRE(sizer.get_account_balance() == Approx(20000.0));

    sizer.set_risk_per_trade(2.5);
    REQUIRE(sizer.get_risk_per_trade() == Approx(2.5));
}
