// Author: Robert Polk
// Copyright (c) 2024 BLINK. All rights reserved.
// Last Modified: 11/8/2024

#include "logger.h"

// Set static inst to null
Logger* Logger::inst = nullptr;

void setLogLevel(const LogLevel& newlogLevel) {
    if (inst == nullptr) {
        inst = new Logger(newlogLevel);
    } else {
        logLevel = newlogLevel;
    }
}

Logger* Logger::instance() {
    if (inst == nullptr) {
        throw std::runtime_error("Logger::setLogLevel must be called first");
    }

    return inst;
}

void debug(const std::string&);

void info(const std::string&);

void warning(const std::string&);

void error(const std::string&);

private:
Logger(LogLevel = INFO);