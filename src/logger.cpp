// Author: Robert Polk
// Copyright (c) 2024 BLINK. All rights reserved.
// Last Modified: 11/8/2024

#include "logger.h"

#ifdef LOGGING_DATA
    void SerialLogger::data(const char *message, const char *tag) {
        printLog("DATA", message, tag);
    }
#endif // LOGGING_DATA

#ifdef LOGGING_DEBUG
    void SerialLogger::debug(const char *message, const char *tag) {
        printLog("DEBUG", message, tag);
    }
#endif // LOGGING_DEBUG

#ifdef LOGGING_INFO
    void SerialLogger::info(const char *message, const char *tag) {
        printLog("INFO", message, tag);
    }
#endif // LOGGING_INFO

#ifdef LOGGING_WARNING
    void SerialLogger::warning(const char *message, const char *tag) {
        printLog("WARNING", message, tag);
    }
#endif // LOGGING_WARNING

#ifdef LOGGING_ERROR
    void SerialLogger::error(const char *message, const char *tag) {
        printLog("ERROR", message, tag);
    }
#endif // LOGGING_ERROR

void SerialLogger::printLog(const char *logLevel, const char *message, const char *tag) {
    Serial.print("[");
    Serial.print(logLevel);
    Serial.print("]: ");
    Serial.print(tag);
    Serial.print(" -> ");
    Serial.println(message);
}