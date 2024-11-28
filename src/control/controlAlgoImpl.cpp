// Author: Robert Polk
// Copyright (c) 2024 BLINK. All rights reserved.
// Last Modified: 11/26/2024

#include "control/controlAlgoImpl.h"

void ControlAlgoImpl::execute() {
    setTargetQuaternion();
    setCurrentQuaternion();
    slerp();
    calculateAngularVelocity();
    applyInverseKinematics();
    PID();
}

ExtendedQuaternion ControlAlgoImpl::setCurrentQuaternion() {
    return {};
}

ExtendedQuaternion ControlAlgoImpl::slerp() {
    return {};
}

void ControlAlgoImpl::calculateAngularVelocity() {
    //todo why am i even calculating angular velo? How do i know what i need to get from quat to
    // velo
}

void ControlAlgoImpl::applyInverseKinematics() {
    // todo whats the math to go from the above to motor commands?
}

void ControlAlgoImpl::PID() {
    // todo figure out how to PID for motors (angular velo? position? etc. Will likely need
    //  encoder data at some point)
}