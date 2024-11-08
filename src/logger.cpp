// Author: Robert Polk
// Copyright (c) 2024 BLINK. All rights reserved.
// Last Modified: 11/8/2024

#include "logger.h"

// Set static inst to null
Logger* Logger::inst = nullptr;

Logger* Logger::instance() {
    if (inst == nullptr) {
        inst = new Logger();
    }

    return inst;
}

void Logger::data(const char *message, const char *tag) {
    printLog(DATA, message, tag);
}

void Logger::debug(const char *message, const char *tag) {
    printLog(DEBUG, message, tag);
}

void Logger::info(const char *message, const char *tag) {
    printLog(INFO, message, tag);
}

void Logger::warning(const char *message, const char *tag) {
    printLog(WARNING, message, tag);
}

void Logger::error(const char *message, const char *tag) {
    printLog(ERROR, message, tag);
}

void Logger::printLog(const LogLevel &logLevel, const char *message, const char *tag) {
    Serial.print("[");
    Serial.print(logLevel);
    Serial.print("]: ");
    Serial.print(tag);
    Serial.print(" -> ");
    Serial.println(message);
}