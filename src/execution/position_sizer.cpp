#include "../include/execution/position_sizer.hpp"
#include "../include/utils/logger.hpp"

#include <cmath>

PositionSizer::PositionSizer(double account_balance, double risk_per_trade_percent)
    : account_balance_(account_balance), risk_per_trade_percent_(risk_per_trade_percent) {}

double PositionSizer::calculate_position_size(double entry_price, double stop_loss_price) const {
    if (entry_price <= 0.0 || stop_loss_price <= 0.0) {
        auto logger = Logger::get_instance();
        logger->error(" Invalid price(s) given for position sizing.");
        return 0.0;
    }

    double risk_amount = (risk_per_trade_percent_ / 100.0) * account_balance_;
    double stop_distance = std::fabs(entry_price - stop_loss_price);

    if (stop_distance == 0) {
        auto logger = Logger::get_instance();
        logger->error(" Stop loss price equals entry price; cannot calculate position size.");
        return 0.0;
    }

    double position_size = risk_amount / stop_distance;

    auto logger = Logger::get_instance();
    logger->info(" Calculated position size: {} units (risk amount: {}, stop distance: {})",
                 position_size, risk_amount, stop_distance);

    return position_size;
}

void PositionSizer::set_account_balance(double balance) {
    account_balance_ = balance;
}

double PositionSizer::get_account_balance() const {
    return account_balance_;
}

void PositionSizer::set_risk_per_trade(double risk_percent) {
    risk_per_trade_percent_ = risk_percent;
}

double PositionSizer::get_risk_per_trade() const {
    return risk_per_trade_percent_;
}
