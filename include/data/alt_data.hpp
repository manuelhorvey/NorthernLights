#pragma once
#include <vector>
#include <string>
#include <chrono>

struct AltDataPoint {
    std::chrono::system_clock::time_point timestamp;
    std::string source;
    std::string text;
    double sentiment_score;
};

class AltData {
public:
    void load_news();

    void add_data_point(const AltDataPoint& point);
    const std::vector<AltDataPoint>& get_data() const;

    double average_sentiment(std::chrono::minutes period) const;
    double average_sentiment(std::chrono::minutes period,
                             std::chrono::system_clock::time_point now) const;

private:
    std::vector<AltDataPoint> data_points_;
};
