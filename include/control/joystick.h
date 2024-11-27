// Author: Robert Polk
// Copyright (c) 2024 BLINK. All rights reserved.
// Last Modified: 11/27/2024

#ifndef JOYSTICK_H
#define JOYSTICK_H

#include <Arduino.h>
#include "controlAlgoImpl.h"

/**
 * This class defines a control algorithm that get user controlled joystick input
 */
class Joystick final : public ControlAlgoImpl {
public:
    // Default copy-constructor, assignment-op, and destructor
    Joystick(const Joystick &) = default;

    Joystick &operator=(const Joystick &) = default;

    ~Joystick() override = default;

    friend class Factory;   // For construction

private:
    Joystick();

    Quaternion getTarget() override;
};

#endif // JOYSTICK_H