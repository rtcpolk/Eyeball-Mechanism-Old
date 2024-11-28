// Author: Robert Polk
// Copyright (c) 2024 BLINK. All rights reserved.
// Last Modified: 11/25/2024

#ifndef CONTROLALGOIMPL_H
#define CONTROLALGOIMPL_H

#include <Arduino.h>
#include <ArduinoLog.h>
#include "control/extendedQuaternion.h"
#include "mechanism/clientHandler.h"
#include "mechanism/motorHandler.h"

/**
 * Abstract base class for rhs of ControlAlgo bridge
 */
class ControlAlgoImpl {
public:
    // Default constructor and virtual destructor
    ControlAlgoImpl() = default;
    virtual ~ControlAlgoImpl() = default;

    // Delete copy-constructor and assignment-op
    ControlAlgoImpl(const ControlAlgoImpl&) = delete;
    ControlAlgoImpl &operator=(const ControlAlgoImpl&) = delete;

    /**
     * Execute the control algo. Provides the common execution interface for derived algos
     */
    void execute();

private:
    virtual ExtendedQuaternion setTargetQuaternion() = 0;
    ExtendedQuaternion setCurrentQuaternion();

    /**
     * https://en.wikipedia.org/wiki/Slerp
     * @return
     */
    ExtendedQuaternion slerp();
    void calculateAngularVelocity();
    void applyInverseKinematics();
    virtual void PID();

    // Member variables

};

#endif // CONTROLALGOIMPL_H