#pragma once

#include <memory>
#include <spdlog/spdlog.h>

class Logger
{
   public:
    static std::shared_ptr<spdlog::logger> get_instance();
};
