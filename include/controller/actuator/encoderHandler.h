// Author: Robert Polk
// Copyright (c) 2024 BLINK. All rights reserved.
// Last Modified: 11/11/2024

#ifndef ENCODERHANDLER_H
#define ENCODERHANDLER_H

#include <Arduino.h>
#include <ArduinoLog.h>

struct Encoder {
    Encoder() = delete;
    Encoder(const uint8_t&, const uint8_t &);
    const uint8_t pinA;
    const uint8_t pinB;
};

class EncoderHandler {
public:
    EncoderHandler(const EncoderHandler &) = delete;

    EncoderHandler &operator=(const EncoderHandler &) = delete;

    ~EncoderHandler();

    static bool initialize(const uint8_t &, const uint8_t &, const uint8_t &, const uint8_t &,
                           const uint8_t &, const uint8_t &);

    static EncoderHandler *instance();


private:
    EncoderHandler(const uint8_t &, const uint8_t &, const uint8_t &, const uint8_t &, const
    uint8_t &, const uint8_t &);

    static EncoderHandler *inst;
    Encoder encoderA;
    Encoder encoderB;
    Encoder encoderC;
};


#endif // ENCODERHANDLER_H