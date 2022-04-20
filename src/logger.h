#pragma once

#include <string>
#include <format>

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
        static void info(const std::string_view &fmt, Args &&... args) {
            log_internal(std::format(fmt, std::forward<Args>(args)...), Level::INFO);
        }

        template<typename... Args>
        static void warn(const std::string_view &fmt, Args &&... args) {
            log_internal(std::format(fmt, std::forward<Args>(args)...), Level::WARNING);
        }

        template<typename... Args>
        static void error(const std::string_view &fmt, Args &&... args) {
            log_internal(std::format(fmt, std::forward<Args>(args)...), Level::ERROR);
        }
    };

}