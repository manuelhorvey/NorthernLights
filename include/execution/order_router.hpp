#pragma once

#include <string>
#include <optional>
#include <mutex>
#include <queue>

struct Order {
    enum class Type { Market, Limit, Stop };
    enum class Side { Buy, Sell };

    std::string symbol;
    Type type;
    Side side;
    double quantity;
    double price;  // Used for Limit and Stop orders
    std::string order_id; // Unique ID assigned when order is sent
};

class OrderRouter {
public:
    OrderRouter();
    ~OrderRouter();

    // Sends an order to the broker/API; returns assigned order ID or nullopt if failed
    std::optional<std::string> send_order(const Order& order);

    // Cancels an order by ID; returns success status
    bool cancel_order(const std::string& order_id);

    // Polls and processes order status updates (stub for now)
    void poll_order_updates();

private:
    std::mutex mtx_;

    // Simulated order book to track sent orders (mock)
    std::queue<Order> order_queue_;

    // Internal method to generate unique order IDs
    std::string generate_order_id();
};
