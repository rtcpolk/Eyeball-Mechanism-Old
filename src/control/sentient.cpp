// Author: Robert Polk
// Copyright (c) 2024 BLINK. All rights reserved.
// Last Modified: 11/27/2024

#include "control/sentient.h"

Sentient::Sentient() : ControlAlgoImpl() {
    Log.traceln("Sentient Created");
}

Quaternion Sentient::getTarget() {
    //todo update
    Log.traceln("Sentient executed");
    return Quaternion();
}