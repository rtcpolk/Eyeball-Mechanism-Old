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
    // Delete copy-constructor and assignment-op
    Joystick(const Joystick &) = delete;

    Joystick &operator=(const Joystick &) = delete;

    // Default destructor
    ~Joystick() override = default;

    friend class Factory;   // For construction

private:
    /**
     * Primary constructor - used by factory
     */
    Joystick();

    Quaternion setTargetQuaternion() override;
};

#endif // JOYSTICK_H