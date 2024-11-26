// Author: Robert Polk
// Copyright (c) 2024 BLINK. All rights reserved.
// Last Modified: 11/25/2024

#ifndef FACTORY_H
#define FACTORY_H

#include <Arduino.h>
#include "controlAlgo.h"

class Factory {
public:
    // Default constructor and destructor
    Factory() = default;
    ~Factory() = default;

    // Delete copy-constructor and assignment-op
    Factory(const Factory&) = delete;
    Factory &operator=(const Factory&) = delete;

    /**
     * Primary factory method
     * @return
     */
    ControlAlgo makeControlAlgo();

private:
    // Make different types of control algos here
    ControlAlgo makeDBT2();
};

#endif // FACTORY_H