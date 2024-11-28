// Author: Robert Polk
// Copyright (c) 2024 BLINK. All rights reserved.
// Last Modified: 11/27/2024

#ifndef SENTIENT_H
#define SENTIENT_H

#include <Arduino.h>
#include "controlAlgoImpl.h"

/**
 * This class defines a control algorithm that is all knowing (tracks faces)
 */
class Sentient final : public ControlAlgoImpl {
public:
    // Delete copy-constructor and assignment-op
    Sentient(const Sentient &) = delete;

    Sentient &operator=(const Sentient &) = delete;

    // Default destructor
    ~Sentient() override = default;

    friend class Factory;   // For construction
private:
    /**
     * Primary constructor - used by factory
     */
    Sentient();

    ExtendedQuaternion setTargetQuaternion() override;
};

#endif // SENTIENT_H