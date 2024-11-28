// Author: Robert Polk
// Copyright (c) 2024 BLINK. All rights reserved.
// Last Modified: 11/25/2024

#ifndef CONTROLALGOIMPL_H
#define CONTROLALGOIMPL_H

#include <Arduino.h>
#include <ArduinoLog.h>
//#include "control/quaternion.h"
#include "mechanism/clientHandler.h"
#include "mechanism/motorHandler.h"
#include "../lib/MPU6050/helper_3dmath.h"

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
    virtual Quaternion setTargetQuaternion() = 0;
    Quaternion setCurrentQuaternion();

    /**
     * https://en.wikipedia.org/wiki/Slerp
     * @return
     */
    Quaternion slerp();
    void calculateAngularVelocity();
    void applyInverseKinematics();
    virtual void PID();

    // Member variables
    Quaternion targetQuat;  // The desired quaternion orientation
    Quaternion currentQuat; // Current quaternion orientation
    static float initialDistance;
};

#endif // CONTROLALGOIMPL_H