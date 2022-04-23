#pragma once

#include <fmt/format.h>
#include <string>
#include <string_view>

namespace glfr {
    enum class Level {
        INFO,
        WARNING,
        ERROR
    };

    class logger {
    private:
        static void log_internal(const std::string &message, Level level);

    public:
        template<typename... Args>
        static void info(const fmt::format_string<Args...> &fmt, Args &&... args) {
            log_internal(fmt::format(fmt, std::forward<Args>(args)...), Level::INFO);
        }

        template<typename... Args>
        static void warn(const fmt::format_string<Args...> &fmt, Args &&... args) {
            log_internal(fmt::format(fmt, std::forward<Args>(args)...), Level::WARNING);
        }

        template<typename... Args>
        static void error(const fmt::format_string<Args...> &fmt, Args &&... args) {
            log_internal(fmt::format(fmt, std::forward<Args>(args)...), Level::ERROR);
        }
    };

}