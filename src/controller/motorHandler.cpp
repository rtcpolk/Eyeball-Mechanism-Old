// Author: Robert Polk
// Copyright (c) 2024 BLINK. All rights reserved.
// Last Modified: 11/24/2024

#include "controller/motorHandler.h"

// Set static inst to null and initialized to false
MotorHandler *MotorHandler::inst = nullptr;

MotorHandler::~MotorHandler() noexcept { inst = nullptr; }

void MotorHandler::initialize(const std::array<std::array<uint8_t, 2>, 3> &driverParams, const uint32_t &PWM_FREQUENCY, const uint8_t &PWM_RESOLUTION) {
    Log.traceln("MotorHandler::initialize - Begin");

    // Can only initialize once
    if (initialized) {
        throw std::runtime_error("MotorHandler::initialize can only be called once");
    }

    // Ensure params are valid
    for (size_t i(0); i < driverParams.size(); ++i) {
        for (size_t j(0); j < driverParams[i].size(); ++j) {
            if (driverParams[i][j] > 39) {
                throw std::logic_error("MotorHandler::initialize - Invalid PWM or DIRECTION_PIN");
            }
        }
    }

    if (PWM_FREQUENCY > 40000000) {
        throw std::logic_error("MotorHandler::initialize - PWM_FREQUENCY exceeds the maximum");
    }

    if (PWM_RESOLUTION < 1 || PWM_RESOLUTION > 16) {
        throw std::logic_error("MotorHandler::initialize - Invalid PWM_RESOLUTION");
    }

    for (size_t i(0); i < drivers.size(); ++i) {
        // Set direction pin values
        drivers[i].directionPin = driverParams[i][0];
        pinMode(drivers[i].directionPin, OUTPUT);
        digitalWrite(drivers[i].directionPin, LOW);

        // Set pwm pin
        drivers[i].pwmPin = driverParams[i][1];
        ledcSetup(i, PWM_FREQUENCY, PWM_RESOLUTION);
        ledcAttachPin(drivers[i].pwmPin, i);
        ledcWrite(i, 0);
    }

    initialized = true;
    Log.infoln("MotorHandler Initialized successfully");
    Log.traceln("MotorHandler::initialize - End");
}

MotorHandler *MotorHandler::instance() {
    if (inst == nullptr) {
        inst = new MotorHandler();
    }

    return inst;
}