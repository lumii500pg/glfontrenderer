#include "logger.h"
#include <iostream>

#include <fmt/os.cc>
#include <fmt/format.cc>

namespace glfr {
    const char *prefix = "Logger >> ";

    const char *level_to_string(Level level) {
        switch (level) {
            case Level::INFO:
                return "Info";
            case Level::WARNING:
                return "Warn";
            case Level::ERROR:
                return "Error";
            default:
                return "Info";
        }
    }

    void logger::log_internal(const std::string &input, Level level) {
        std::cout << "[" << level_to_string(level) << "] " << prefix << input << std::endl;
    }
}
