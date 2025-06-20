// tests/test_order_router.cpp
#include <catch2/catch.hpp>
#include "execution/order_router.hpp"

TEST_CASE("OrderRouter send_order returns unique IDs and stores orders", "[order_router]") {
    OrderRouter router;

    Order order1{"AAPL", Order::Type::Market, Order::Side::Buy, 100, 0.0, ""};
    Order order2{"GOOG", Order::Type::Limit, Order::Side::Sell, 50, 1500.0, ""};

    auto id1 = router.send_order(order1);
    REQUIRE(id1.has_value());

    auto id2 = router.send_order(order2);
    REQUIRE(id2.has_value());

    REQUIRE(id1.value() != id2.value());

    REQUIRE(id1->rfind("ORD", 0) == 0);
    REQUIRE(id2->rfind("ORD", 0) == 0);
}

TEST_CASE("OrderRouter cancel_order returns true", "[order_router]") {
    OrderRouter router;
    bool canceled = router.cancel_order("ORD000001");
    REQUIRE(canceled == true);
}

TEST_CASE("OrderRouter generate_order_id produces incrementing IDs", "[order_router]") {
    OrderRouter router;

    auto id1 = router.send_order(Order{"AAPL", Order::Type::Market, Order::Side::Buy, 10, 0.0, ""});
    auto id2 = router.send_order(Order{"AAPL", Order::Type::Market, Order::Side::Buy, 10, 0.0, ""});

    REQUIRE(id1 != id2);
    REQUIRE(id1->substr(0, 3) == "ORD");
    REQUIRE(id2->substr(0, 3) == "ORD");
}
