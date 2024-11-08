// Author: Robert Polk
// Copyright (c) 2024 BLINK. All rights reserved.
// Last Modified: 11/08/2024

#ifndef LOGGER_H
#define LOGGER_H

#include <Arduino.h>

class Logger {
public:
    enum LogLevel {
        DEBUG,
        INFO,
        WARNING,
        ERROR
    };

    void setLogLevel(const LogLevel&);

    static Logger* instance();

    void debug(const std::string&);

    void info(const std::string&);

    void warning(const std::string&);

    void error(const std::string&);

private:
    Logger(LogLevel = INFO);

    // Member variables
    static Logger* inst;    // Singleton ptr
    LogLevel logLevel;  // Current LogLevel
};


#endif // LOGGER_H