#include <catch2/catch.hpp>
#include <chrono>
#include "data/alt_data.hpp"

using namespace std::chrono;

TEST_CASE("AltData average sentiment works over 5 minutes", "[AltData]")
{
    AltData alt;
    auto    base = system_clock::from_time_t(0);

    alt.add_data_point({base - minutes(10), "news", "Old negative news", -0.5});
    alt.add_data_point({base - minutes(3), "social", "Recent hype!", 0.9});
    alt.add_data_point({base - minutes(2), "news", "Another positive", 0.8});
    alt.add_data_point({base - minutes(1), "news", "Short term mood swing", -0.2});

    double avg = alt.average_sentiment(minutes(5), base);
    REQUIRE(avg == Approx((0.9 + 0.8 - 0.2) / 3.0));
}

TEST_CASE("AltData average sentiment works over 2 minutes", "[AltData]")
{
    AltData alt;
    auto    base = system_clock::from_time_t(0);

    alt.add_data_point({base - minutes(2), "news", "Another positive", 0.8});
    alt.add_data_point({base - minutes(1), "news", "Short term mood swing", -0.2});

    double avg = alt.average_sentiment(minutes(2), base);
    REQUIRE(avg == Approx((0.8 - 0.2) / 2.0));
}

TEST_CASE("AltData average sentiment returns 0 for 0-minute window", "[AltData]")
{
    AltData alt;
    auto    base = system_clock::from_time_t(0);

    alt.add_data_point({base - minutes(1), "news", "Short term mood swing", -0.2});
    double avg = alt.average_sentiment(minutes(0), base);
    REQUIRE(avg == Approx(0.0));
}

TEST_CASE("AltData average sentiment returns 0 for empty dataset", "[AltData]")
{
    AltData alt;
    auto    base = system_clock::from_time_t(0);
    REQUIRE(alt.average_sentiment(minutes(5), base) == Approx(0.0));
}
