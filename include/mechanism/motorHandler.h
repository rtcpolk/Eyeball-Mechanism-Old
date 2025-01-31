// Author: Robert Polk
// Copyright (c) 2024 BLINK. All rights reserved.
// Last Modified: 01/30/25

#ifndef MOTORHANDLER_H
#define MOTORHANDLER_H

//#define DISABLE_LOGGING

#include <Arduino.h>
#include <ArduinoLog.h>
#include <array>

struct MotorDriver {
    // Movement methods
    void forward() const;
    void backward() const;
    void stop() const;

    // Member variables
    uint8_t pinA; // PWM signal pin connected to IN1
    uint8_t pinB;   // Direction signal pin connected to IN2
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
    * Print out a list of valid commands with descriptions
    */
    static void help() ;

    // Movement methods
    void forward() const;
    void backward() const;
    void stop() const;
    void moveLoop() const;
    void test() const;

private:
    // Primary constructor
    MotorHandler() = default;

    // Member variables
    static MotorHandler *inst;  // Ptr to the singleton inst
    static bool initialized;    // Initialization flag
    std::array<MotorDriver, 3> drivers; // Array to hold the motor drivers
    uint8_t resolution; // The resolution of the PWM duty cycle
    uint32_t delayTime; // The delay time during the movement loop
};

#endif // MOTORHANDLER_H