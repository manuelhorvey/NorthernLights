#include "../include/utils/logger.hpp"
#include <spdlog/sinks/basic_file_sink.h>
#include <spdlog/sinks/stdout_color_sinks.h>
#include <filesystem>

std::shared_ptr<spdlog::logger> Logger::get_instance() {
    static std::shared_ptr<spdlog::logger> logger = [] {
        // Ensure logs directory exists
        std::filesystem::create_directories("logs");

        // Setup sinks: console + file
        auto console_sink = std::make_shared<spdlog::sinks::stdout_color_sink_mt>();
        auto file_sink = std::make_shared<spdlog::sinks::basic_file_sink_mt>("logs/execution.log", true);

        console_sink->set_pattern("[%^%l%$] %v");
        file_sink->set_pattern("[%Y-%m-%d %H:%M:%S] [%l] %v");

        auto combined_logger = std::make_shared<spdlog::logger>("main_logger", spdlog::sinks_init_list{console_sink, file_sink});
        spdlog::register_logger(combined_logger);

        combined_logger->set_level(spdlog::level::info);
        combined_logger->flush_on(spdlog::level::info);

        return combined_logger;
    }();

    return logger;
}
