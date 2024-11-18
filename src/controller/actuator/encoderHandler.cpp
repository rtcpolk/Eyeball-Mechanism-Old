// Author: Robert Polk
// Copyright (c) 2024 BLINK. All rights reserved.
// Last Modified: 11/11/2024

#include "controller/actuator/encoderHandler.h"

Encoder::Encoder(const uint8_t &pinA, const uint8_t &pinB) : pinA(pinA), pinB(pinB) {}

// Set static inst to null
EncoderHandler *EncoderHandler::inst = nullptr;

EncoderHandler::~EncoderHandler() {
    Log.traceln("EncoderHandler::~EncoderHandler - Start");
    inst = nullptr;
    Log.traceln("EncoderHandler::~EncoderHandler - Stop");
}

bool EncoderHandler::initialize(const uint8_t &FIRST_PIN_A, const uint8_t &FIRST_PIN_B, const
uint8_t &SECOND_PIN_A, const uint8_t &SECOND_PIN_B, const uint8_t &THIRD_PIN_A, const uint8_t
                                &THIRD_PIN_B) {
    Log.traceln("EncoderHandler::initialize - Start");

    // Only initialize once
    if (inst != nullptr) {
        Log.errorln("ClientHandler::initialize - Already initialized");
        Log.trace("ClientHandler::initialzie - Stop");
        return false;
    }

    // Attempt to allocate memory for the singleton inst
    inst = new(std::nothrow) EncoderHandler(FIRST_PIN_A, FIRST_PIN_B, SECOND_PIN_A, SECOND_PIN_B,
                                            THIRD_PIN_A, THIRD_PIN_B);
    if (!inst) {
        Log.errorln("EncoderHandler::initialize - Memory allocation failed for EncoderHandler "
                    "instance");
        Log.traceln("EncoderHandler::initialize - Stop");
        return false;
    }

    Log.infoln("EncoderHandler::initialize - EncoderHandler initialized successfully");
    Log.traceln("EncoderHandler::initialize - Stop");
    return true;
}

EncoderHandler *EncoderHandler::instance() {
    Log.traceln("EncoderHandler::instance - Start");

    if (inst == nullptr) {
        Log.errorln("EncoderHandler::instance - Initialize must be called first");
    }

    Log.traceln("EncoderHandler::instance - Stop");
    return inst;
}

EncoderHandler::EncoderHandler(const uint8_t &FIRST_PIN_A, const uint8_t &FIRST_PIN_B, const
uint8_t &SECOND_PIN_A, const uint8_t &SECOND_PIN_B, const uint8_t &THIRD_PIN_A, const uint8_t
                               &THIRD_PIN_B) : encoderA(FIRST_PIN_A, FIRST_PIN_B), encoderB
        (SECOND_PIN_A, SECOND_PIN_B), encoderC(THIRD_PIN_A, THIRD_PIN_B) {}