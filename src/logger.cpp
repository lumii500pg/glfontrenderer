//
// Created by lu on 20/04/2022.
//

#include "logger.h"
#include <iostream>

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