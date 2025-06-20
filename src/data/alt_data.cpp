#include "../include/data/alt_data.hpp"
#include <iostream>

void AltData::load_news()
{
    std::cout << "📰 AltData loading mock macro/news events...\n";

    // Mock data
    AltDataPoint mock_news{
        std::chrono::system_clock::now(), "Reuters", "Central bank hints at policy tightening",
        0.65  // Positive sentiment
    };

    add_data_point(mock_news);
}

void AltData::add_data_point(const AltDataPoint& point)
{
    data_points_.push_back(point);
}

const std::vector<AltDataPoint>& AltData::get_data() const
{
    return data_points_;
}

double AltData::average_sentiment(std::chrono::minutes period) const
{
    return average_sentiment(period, std::chrono::system_clock::now());
}

double AltData::average_sentiment(std::chrono::minutes                  period,
                                  std::chrono::system_clock::time_point now) const
{
    auto   cutoff = now - period;
    double sum    = 0.0;
    int    count  = 0;

    for (const auto& point : data_points_)
    {
        if (point.timestamp >= cutoff)
        {
            sum += point.sentiment_score;
            ++count;
        }
    }

    return count > 0 ? sum / count : 0.0;
}
