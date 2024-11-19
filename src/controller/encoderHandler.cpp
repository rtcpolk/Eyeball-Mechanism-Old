// Author: Robert Polk
// Copyright (c) 2024 BLINK. All rights reserved.
// Last Modified: 11/19/2024

#include "controller/encoderHandler.h"

// Set static inst to null and initialized to false
EncoderHandler *EncoderHandler::inst = nullptr;
bool EncoderHandler::initialized = false;

EncoderHandler::~EncoderHandler() noexcept { inst = nullptr; }

void EncoderHandler::initialize(const std::array<std::array<uint8_t, 2>, 3> &pins) {
    Log.traceln("EncoderHandler::initialize - Begin");

    // Only initialize once
    if (initialized) {
        throw std::runtime_error("EncoderHandler::initialize can only be called once");
    }

    // Attach and make sure its successful
    ESP32Encoder::useInternalWeakPullResistors = puType::up;
    try {
        for (size_t i(0); i < encoders.size(); ++i) {
            encoders[i].attachFullQuad(pins[i][0], pins[i][1]);
            encoders[i].clearCount();
        }
    } catch (...) {
        throw;
    }

    initialized = true;
    Log.infoln("EncoderHandler::initialize - EncoderHandler initialized successfully");
    Log.traceln("EncoderHandler::initialize - End");
}

EncoderHandler *EncoderHandler::instance() {
    if (inst == nullptr) {
        inst = new EncoderHandler();
    }

    return inst;
}

const std::array<int64_t, 3> &EncoderHandler::getCounts() const noexcept {
    return counts;
}

void EncoderHandler::loop() {
    while (true) {
        EncoderHandler::instance()->updateCounts();
        delay(100); // todo optimize this
    }
}

EncoderHandler::EncoderHandler() : counts{0, 0, 0} {}

void EncoderHandler::updateCounts() noexcept {
    Log.traceln("EncoderHandler::updateCounts - Begin");

    for (size_t i(0); i < encoders.size(); ++i) {
        counts[i] = encoders[i].getCount();
    }

    Log.verboseln("\tEncoder Counts:\t%d\t%d\t%d", counts[0], counts[1], counts[2]);
    Log.traceln("EncoderHandler::updateCounts - End");
}

void EncoderHandler::resetCounts() noexcept {
    Log.traceln("EncoderHandler::resetCounts - Begin");

    for (size_t i(0); i < encoders.size(); ++i) {
        int64_t temp = counts[i];
        uint8_t result = encoders[i].clearCount();

        // Ensure reset was successful
        if (result != 0) {
            Log.warningln("EncoderHandler::resetCounts - clearCount failed. Resetting counts");
            encoders[i].setCount(temp);
        } else {
            counts[i] = 0;
        }
    }

    Log.verboseln("\tEncoder Counts:\t%d\t%d\t%d", counts[0], counts[1], counts[2]);
    Log.traceln("EncoderHandler::resetCounts - End");
}