// Author: Robert Polk
// Copyright (c) 2024 BLINK. All rights reserved.
// Last Modified: 11/25/2024

#ifndef CONTROLALGOIMPL_H
#define CONTROLALGOIMPL_H

#include <Arduino.h>
#include <ArduinoLog.h>
#include <../lib/MPU6050/helper_3dmath.h>

/**
 * Abstract base class for rhs of ControlAlgo bridge
 */
class ControlAlgoImpl {
public:
    // Default constructor and virtual destructor
    ControlAlgoImpl() = default;
    virtual ~ControlAlgoImpl() = default;

    bool execute();

private:
    virtual Quaternion getTarget() = 0;
};

#endif // CONTROLALGOIMPL_H




















































///**
// * Abstract base class for rhs of ControlAlgo bridge
// */
//class ControlAlgoImpl {
//public:
//    // Default constructor, copy-constructor, assignment-op, and destructor
//    ControlAlgoImpl() = default;
//
//    ControlAlgoImpl(const ControlAlgoImpl&) = default;
//
//    ControlAlgoImpl &operator=(const ControlAlgoImpl &) = default;
//
//    virtual ~ControlAlgoImpl() = default;
//
//    /**
//     * Execution interface for derived control algos
//     *
//     * @return True if successful
//     */
//    bool execute();
//
//private:
//    // Private methods that define the control algo...which need to be overriden?
//    virtual Quaternion getTarget() = 0;
//
//};
//
//#endif // CONTROLALGOIMPL_H