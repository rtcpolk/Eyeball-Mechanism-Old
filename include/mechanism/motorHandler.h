// Author: Robert Polk
// Copyright (c) 2024 BLINK. All rights reserved.
// Last Modified: 11/24/2024

#ifndef MOTORHANDLER_H
#define MOTORHANDLER_H

#define DISABLE_LOGGING

#include <Arduino.h>
#include <ArduinoLog.h>
#include <array>

struct MotorDriver {
    uint8_t pwmPin; // PWM signal pin connected to IN1
    uint8_t directionPin;   // Direction signal pin connected to IN2
};

class MotorHandler {
public:
    // Delete constructor, copy-constructor and assignment-op
    MotorHandler(const MotorHandler &) = delete;

    MotorHandler &operator=(const MotorHandler &) = delete;

    // Destructor
    ~MotorHandler() noexcept;

    static MotorHandler *instance();

    /**
     * Initialize the Motor Handler by setting the motor driver pins
     * @param motorPins - An array that holds all of the pins for the motor drivers
     *                    {Direction Pin, PWM Pin}
     * @param PWM_FREQUENCY - The frequency of the PWM signal
     * @param PWM_RESOLUTION - The resolution of the PWM duty cycle
     */
    void initialize(const std::array<std::array<uint8_t, 2>, 3> &motorPins, const uint32_t
    &PWM_FREQUENCY, const uint8_t &PWM_RESOLUTION);

    /**
     * Set the speeds of the 3 motors
     *
     * @param speeds - An array holding the pwm values
     */
    void setMotorSpeeds(const std::array<int16_t, 3> &speeds);

private:
    // Primary constructor
    MotorHandler() = default;

    // Member variables
    static MotorHandler *inst;  // Ptr to the singleton inst
    static bool initialized;    // Initialization flag
    std::array<MotorDriver, 3> drivers; // Array to hold the motor drivers
    uint8_t resolution; // The resolution of the PWM duty cycle
};

#endif // MOTORHANDLER_H