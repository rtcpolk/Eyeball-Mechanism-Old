// Author: Robert Polk
// Copyright (c) 2024 BLINK. All rights reserved.
// Last Modified: 11/8/2024

#include "logger.h"

void SerialLogger::data(const char *message, const char *tag) {
    printLog("DATA", message, tag);
}


void SerialLogger::debug(const char *message, const char *tag) {
    printLog("DEBUG", message, tag);
}


void SerialLogger::info(const char *message, const char *tag) {
    printLog("INFO", message, tag);
}


void SerialLogger::warning(const char *message, const char *tag) {
    printLog("WARNING", message, tag);
}


void SerialLogger::error(const char *message, const char *tag) {
    printLog("ERROR", message, tag);
}

void SerialLogger::printLog(const char *logLevel, const char *message, const char *tag) {
    Serial.print("[");
    Serial.print(logLevel);
    Serial.print("]: ");
    Serial.print(tag);
    Serial.print(" -> ");
    Serial.println(message);
}