#include "../include/utils/logger.hpp"
#include <spdlog/sinks/basic_file_sink.h>

std::shared_ptr<spdlog::logger> Logger::get_instance() {
    static auto logger = spdlog::basic_logger_mt("main_logger", "logs/execution.log");
    logger->set_level(spdlog::level::info);
    logger->flush_on(spdlog::level::info);
    return logger;
}
