#pragma once

#include <string>

class PositionSizer
{
   public:
    PositionSizer(double account_balance, double risk_per_trade_percent);

    // Calculate position size (quantity of units) given entry price and stop loss price
    double calculate_position_size(double entry_price, double stop_loss_price) const;

    // Setters and getters for account balance and risk percent
    void   set_account_balance(double balance);
    double get_account_balance() const;

    void   set_risk_per_trade(double risk_percent);
    double get_risk_per_trade() const;

   private:
    double account_balance_;         // Total capital available
    double risk_per_trade_percent_;  // % of capital risked per trade (e.g., 1.0 = 1%)
};
