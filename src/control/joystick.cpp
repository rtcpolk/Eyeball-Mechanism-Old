// Author: Robert Polk
// Copyright (c) 2024 BLINK. All rights reserved.
// Last Modified: 11/27/2024

#include "control/joystick.h"

Joystick::Joystick() : ControlAlgoImpl() {
    Log.traceln("joystick Created");
}

Quaternion Joystick::getTarget() {
    //todo update
    Log.traceln("joystick executed");
    return Quaternion();
}