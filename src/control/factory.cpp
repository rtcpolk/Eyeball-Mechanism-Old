// Author: Robert Polk
// Copyright (c) 2024 BLINK. All rights reserved.
// Last Modified: 11/25/2024

#include "control/factory.h"

ControlAlgo Factory::makeControlAlgo(const std::array<uint8_t, 3> &switchInput) {
    if (switchInput[0] == 1) {
        return makeDBT2();
    } else if (switchInput[1] == 1) {
        return makePathFollowing();
    } else if (switchInput[2] == 1) {
        return makeJoystick();
    } else {
        return makeSentient();
    }
}

ControlAlgo Factory::makeDBT2() {
   return ControlAlgo(new DBT2());
}

ControlAlgo Factory::makePathFollowing() {
    return ControlAlgo(new PathFollowing());
}

ControlAlgo Factory::makeJoystick() {
    return ControlAlgo(new Joystick());
}

ControlAlgo Factory::makeSentient() {
    return ControlAlgo(new Sentient());
}