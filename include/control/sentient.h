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
    // Default copy-constructor, assignment-op, and destructor
    Sentient(const Sentient &) = default;
    Sentient&operator=(const Sentient&) = default;
    ~Sentient() override = default;

    friend class Factory;   // For construction
private:
    Sentient();

    Quaternion getTarget() override;
};

#endif // SENTIENT_H