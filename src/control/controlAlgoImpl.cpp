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

Quaternion ControlAlgoImpl::setCurrentQuaternion() {
    return ClientHandler::instance()->getQuaternion();
}

Quaternion ControlAlgoImpl::slerp() {
    // Normalize the quats
    currentQuat.normalize();
    targetQuat.normalize();

    // Compute the dot product of the quats
    float dot = (currentQuat.w * targetQuat.w) + (currentQuat.x * targetQuat.x) + (currentQuat.y
            * targetQuat.y) + (currentQuat.z * targetQuat.z);

    // Calculate the interpolation factor, t
    float currentDistance = 1.0f - fabs(dot);
    float t = (initialDistance - currentDistance) / initialDistance;
    t = constrain(t, 0.0f, 1.0f);

    // If negative negate teh target to take the shorter path
    if (dot < 0.0f) {
        targetQuat = targetQuat;
        dot = -dot;
    }

    dot = constrain(dot, 0.0f, 1.0f);

    float omega = acos(dot);
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