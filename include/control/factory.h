// Author: Robert Polk
// Copyright (c) 2024 BLINK. All rights reserved.
// Last Modified: 11/25/2024

#ifndef FACTORY_H
#define FACTORY_H

#include <Arduino.h>
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
     Factory(const Factory&) = delete;
     Factory &operator=(const Factory&) = delete;

     ControlAlgo makeControlAlgo(const std::array<uint8_t, 3> &switchInput);

 private:
     ControlAlgo makeDBT2();

     ControlAlgo makePathFollowing();

     ControlAlgo makeJoystick();

     ControlAlgo makeSentient();
 };

#endif // FACTORY_H





//class Factory {
//public:
//    // Default constructor and destructor
//    Factory() = default;
//    ~Factory() = default;
//
//    // Delete copy-constructor and assignment-op
//    Factory(const Factory&) = delete;
//    Factory &operator=(const Factory&) = delete;
//
//    /**
//     * Primary factory method
//     * @return
//     */
//    ControlAlgo makeControlAlgo();
//
//private:
//    // Make different types of control algos here
//    ControlAlgo makeDBT2();
//};
//
//#endif // FACTORY_H