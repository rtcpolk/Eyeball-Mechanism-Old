// Author: Robert Polk
// Copyright (c) 2024 BLINK. All rights reserved.
// Last Modified: 11/25/2024

#ifndef FACTORY_H
#define FACTORY_H

//#define DISABLE_LOGGING

#include <Arduino.h>
#include <ArduinoLog.h>
#include "controlAlgo.h"
#include "control/DBT2.h"
#include "control/pathFollowing.h"
#include "control/joystick.h"
#include "control/sentient.h"

/**
 * Factory for choosing controlAlgos from switch input
 */
class Factory {
public:
    // Default constructor and destructor
    Factory() = default;

    ~Factory() = default;

    // Delete copy-constructor and assignment-op
    Factory(const Factory &) = delete;

    Factory &operator=(const Factory &) = delete;

    /**
     * Main factory method that parses switch inputs to create control algos
     *
     * @param switchInput - The switch inputs
     * @return The control algo
     */
    ControlAlgo makeControlAlgo(const std::array<uint8_t, 3> &switchInput);

private:
    /**
     * Make a DBT2 control algo
     * @return - The control algo
     */
    static ControlAlgo makeDBT2();

    /**
     * Make a pathFollowing control algo
     * @return - The control algo
     */
    static ControlAlgo makePathFollowing();

    /**
     * Make a joystick control algo
     * @return - The control algo
     */
    static ControlAlgo makeJoystick();

    /**
     * Make a sentient control algo
     * @return - The control algstatic o
     static */
    static ControlAlgo makeSentient();
};

#endif // FACTORY_H