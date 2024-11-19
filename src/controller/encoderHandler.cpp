// Author: Robert Polk
// Copyright (c) 2024 BLINK. All rights reserved.
// Last Modified: 11/18/2024

#include "controller/actuator/encoderHandler.h"

Encoder::Encoder(const uint8_t &PIN_A, const uint8_t &PIN_B) : pinA(PIN_A), pinB(PIN_B) {
    Log.traceln("Encoder::Encoder - Encoder created");
}

// Set static inst to null
EncoderHandler *EncoderHandler::inst = nullptr;

EncoderHandler::~EncoderHandler() { inst = nullptr; }

void EncoderHandler::initialize(const uint8_t &FIRST_ENCODER_PIN_A,
                                const uint8_t &FIRST_ENCODER_PIN_B,
                                const uint8_t &SECOND_ENCODER_PIN_A,
                                const uint8_t &SECOND_ENCODER_PIN_B,
                                const uint8_t &THIRD_ENCODER_PIN_A,
                                const uint8_t &THIRD_ENCODER_PIN_B) {
    Log.traceln("EncoderHandler::initialize - Begin");

    // Only initialize once
    if (inst != nullptr) {
        throw std::runtime_error("EncoderHandler::initialize can only be called once");
    }

    inst = new EncoderHandler(FIRST_ENCODER_PIN_A, FIRST_ENCODER_PIN_B, SECOND_ENCODER_PIN_A,
                              SECOND_ENCODER_PIN_B, THIRD_ENCODER_PIN_A, THIRD_ENCODER_PIN_B);

    Log.infoln("EncoderHandler::initialize - EncoderHandler initialized successfully");
    Log.trace("EncoderHandler::initialize - End");
}

EncoderHandler *EncoderHandler::instance() {
    if (inst == nullptr) {
        Log.errorln("EncoderHandler::instance - Initialize must be called first");
        throw std::runtime_error("EncoderHandler::instance - Initialize must be called first");
    }

    return inst;
}

EncoderHandler::EncoderHandler(const uint8_t &FIRST_ENCODER_PIN_A,
                               const uint8_t &FIRST_ENCODER_PIN_B,
                               const uint8_t &SECOND_ENCODER_PIN_A,
                               const uint8_t &SECOND_ENCODER_PIN_B,
                               const uint8_t &THIRD_ENCODER_PIN_A,
                               const uint8_t &THIRD_ENCODER_PIN_B) :
        firstEncoder(FIRST_ENCODER_PIN_A, FIRST_ENCODER_PIN_B),
        secondEncoder(SECOND_ENCODER_PIN_A, SECOND_ENCODER_PIN_B),
        thirdEncoder(THIRD_ENCODER_PIN_A, THIRD_ENCODER_PIN_B) {}