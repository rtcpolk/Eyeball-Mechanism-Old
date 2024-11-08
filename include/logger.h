// Author: Robert Polk
// Copyright (c) 2024 BLINK. All rights reserved.
// Last Modified: 11/08/2024

//#ifdef LOGGING
    #ifndef LOGGER_H
    #define LOGGER_H

    #include <Arduino.h>

    class Logger {
    public:
        enum LogLevel {
            DATA,
            DEBUG,
            INFO,
            WARNING,
            ERROR
        };

        static Logger* instance();

        void data(const char*, const char*);

        void debug(const char*, const char*);

        void info(const char*, const char*);

        void warning(const char*, const char*);

        void error(const char*, const char*);

    private:
        Logger() = default;

        static void printLog(const LogLevel&, const char*);

        // Member variables
        static Logger* inst;    // Singleton ptr
    };


    #endif // LOGGER_H
//#endif // LOGGING