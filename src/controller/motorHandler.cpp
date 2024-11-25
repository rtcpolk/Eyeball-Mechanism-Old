// Author: Robert Polk
// Copyright (c) 2024 BLINK. All rights reserved.
// Last Modified: 11/24/2024

#include "controller/motorHandler.h"

// Set static inst to null and initialized to false
MotorHandler *MotorHandler::inst = nullptr;

MotorHandler::~MotorHandler() noexcept { inst = nullptr; }

MotorHandler *MotorHandler::instance() {
    if (inst == nullptr) {
        inst = new MotorHandler();
    }

    return inst;
}

void MotorHandler::initialize(const std::array<std::array<uint8_t, 2>, 3> &motorPins, const uint32_t &PWM_FREQUENCY, const uint8_t &PWM_RESOLUTION) {
    Log.traceln("MotorHandler::initialize - Begin");

    // Can only initialize once
    if (initialized) {
        throw std::runtime_error("MotorHandler::initialize can only be called once");
    }

    // Ensure params are valid
    for (size_t i(0); i < motorPins.size(); ++i) {
        for (size_t j(0); j < motorPins[i].size(); ++j) {
            if (motorPins[i][j] > 39) {
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
    resolution = PWM_RESOLUTION;

    for (size_t i(0); i < drivers.size(); ++i) {
        // Set direction pin values
        drivers[i].directionPin = motorPins[i][0];
        pinMode(drivers[i].directionPin, OUTPUT);
        digitalWrite(drivers[i].directionPin, LOW);

        // Set pwm pin
        drivers[i].pwmPin = motorPins[i][1];
        ledcSetup(i, PWM_FREQUENCY, PWM_RESOLUTION);
        ledcAttachPin(drivers[i].pwmPin, i);
        ledcWrite(i, 0);
    }

    initialized = true;
    Log.infoln("MotorHandler Initialized successfully");
    Log.traceln("MotorHandler::initialize - End");
}

void MotorHandler::setMotorSpeeds(const std::array<int16_t, 3> &speeds) {
    uint16_t maxDutyCycle = (1 << resolution) - 1;

    for (size_t i(0); i < speeds.size(); ++i) {
        // Determine dutyCycle and direction
        uint16_t dutyCycle = abs(constrain(speeds[i], -maxDutyCycle, maxDutyCycle));
        uint8_t direction = speeds[i] >= 0 ? LOW : HIGH;

        // Set the pins
        digitalWrite(drivers[i].directionPin, direction);
        ledcWrite(i, dutyCycle);
    }
}