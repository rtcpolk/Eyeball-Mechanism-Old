// Author: Robert Polk
// Copyright (c) 2024 BLINK. All rights reserved.
// Last Modified: 01/30/25

#include "mechanism/motorHandler.h"

void MotorDriver::forward() const {
    digitalWrite(pinA, HIGH);
}

void MotorDriver::backward() const {
    digitalWrite(pinB, HIGH);
}

void MotorDriver::stop() const {
    if (pinA == HIGH) {
        digitalWrite(pinA, LOW);
        digitalWrite(pinB, LOW);
    } else {
        digitalWrite(pinB, LOW);
        digitalWrite(pinA, LOW);
    }
}

// ===============================================================================================//

// Set static inst to null and initialized to false
MotorHandler *MotorHandler::inst = nullptr;
bool MotorHandler::initialized = false;

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
    for (auto motorPin : motorPins) {
        for (unsigned char j : motorPin) {
            if (j > 39) {
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
        // Set pinA
        drivers[i].pinA = motorPins[i][0];
        pinMode(drivers[i].pinA, OUTPUT);
        digitalWrite(drivers[i].pinA, LOW);

        // Set pinB
        drivers[i].pinB = motorPins[i][1];
        pinMode(drivers[i].pinA, OUTPUT);
        digitalWrite(drivers[i].pinA, LOW);
    }

    initialized = true;
    Log.infoln("MotorHandler Initialized successfully");
    Log.traceln("MotorHandler::initialize - End");
}

void MotorHandler::help() {
    Serial.println();
    Serial.println("'f' : forward - spin all three motors forward");
    Serial.println("'b' : backward - spin all three motors backward");
    Serial.println("'s' : stop - stop all three motors");
    Serial.println("l : loop - run the movement loop");
    Serial.println("t : test - run the test");
    Serial.println("'h' : help - print out a list of valid commands");
    Serial.println("'x' : exit - exit the program and restart the ESP32");
}

void MotorHandler::forward() const {
    stop();

    for (auto driver : drivers) {
        driver.forward();
    }
}

void MotorHandler::backward() const {
    stop();

    for (auto driver : drivers) {
        driver.backward();
    }
}

void MotorHandler::stop() const {
    for (auto driver : drivers) {
        driver.stop();
    }
}

void MotorHandler::moveLoop() const {
    stop();
    forward();
    delay(delayTime);
    stop();
    delay(delayTime);

    backward();
    delay(delayTime);
    stop();
    delay(delayTime);

    drivers[0].forward();
    drivers[1].forward();
    drivers[2].backward();
    delay(delayTime);
    stop();
    delay(delayTime);

    drivers[0].backward();
    drivers[1].backward();
    drivers[2].forward();
    delay(delayTime);
    stop();
    delay(delayTime);

    drivers[0].forward();
    drivers[1].forward();
    delay(delayTime);
    stop();
    delay(delayTime);

    drivers[0].backward();
    drivers[1].backward();
    delay(delayTime);
    stop();
    delay(delayTime);

    drivers[0].forward();
    drivers[2].forward();
    delay(delayTime);
    stop();
    delay(delayTime);

    drivers[0].backward();
    drivers[2].backward();
    delay(delayTime);
    stop();
    delay(delayTime);
}

void MotorHandler::test() const {
    stop();
    //todo what tests do i want
}