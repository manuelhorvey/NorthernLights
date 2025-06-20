#include "../include/execution/order_router.hpp"
#include "../include/utils/logger.hpp"

#include <atomic>
#include <chrono>
#include <iomanip>
#include <sstream>
#include <thread>

static std::atomic<int> order_id_counter{0};

OrderRouter::OrderRouter()  = default;
OrderRouter::~OrderRouter() = default;

std::optional<std::string> OrderRouter::send_order(const Order& order)
{
    std::lock_guard<std::mutex> lock(mtx_);

    std::string id           = generate_order_id();
    Order       routed_order = order;
    routed_order.order_id    = id;

    // Simulate sending order to broker (in real system, replace this with HTTP or FIX)
    order_queue_.push(routed_order);

    auto logger = Logger::get_instance();
    logger->info(" Order sent: {} {} {} @ {} (Type: {}, ID: {})",
                 (order.side == Order::Side::Buy ? "Buy" : "Sell"), order.quantity, order.symbol,
                 order.price, (order.type == Order::Type::Market ? "Market" : "Limit"), id);

    return id;
}

bool OrderRouter::cancel_order(const std::string& order_id)
{
    std::lock_guard<std::mutex> lock(mtx_);

    // In a real system, you'd send a cancel request via broker API here
    auto logger = Logger::get_instance();
    logger->info(" Cancel request sent for Order ID: {}", order_id);

    return true;  // Mock always-successful cancellation
}

void OrderRouter::poll_order_updates()
{
    std::lock_guard<std::mutex> lock(mtx_);

    // In a real system, this would poll the broker for updates like fills, partials, rejections
    auto logger = Logger::get_instance();
    logger->info(" Polling order updates... (stub)");

    // Optional: simulate fills
    // std::this_thread::sleep_for(std::chrono::milliseconds(100));
    // logger->info(" Mock fill confirmed for Order ID: ORD000123");
}

std::string OrderRouter::generate_order_id()
{
    int               id = order_id_counter.fetch_add(1);
    std::stringstream ss;
    ss << "ORD" << std::setw(6) << std::setfill('0') << id;
    return ss.str();
}
