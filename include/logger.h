// Author: Robert Polk
// Copyright (c) 2024 BLINK. All rights reserved.
// Last Modified: 11/08/2024

#ifdef LOGGING
    #ifndef LOGGER_H
    #define LOGGER_H

    #include <Arduino.h>

    class SerialLogger {
    public:
        #ifdef LOGGING_DATA
            static void data(const char*, const char*);
        #endif // LOGGING_DATA

        #ifdef LOGGING_DEBUG
            static void debug(const char*, const char*);
        #endif // LOGGING_DEBUG

        #ifdef LOGGING_INFO
            static void info(const char*, const char*);
        #endif // LOGGING_INFO

        #ifdef LOGGING_WARNING
            static void warning(const char*, const char*);
        #endif // LOGGING_WARNING

        #ifdef LOGGING_ERROR
            static void error(const char*, const char*);
        #endif // LOGGING_ERROR

    private:
        SerialLogger() = default;

        static void printLog(const char *, const char*, const char*);
    };

    #endif // LOGGER_H
#endif // LOGGING