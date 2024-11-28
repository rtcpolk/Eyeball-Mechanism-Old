// Author: Robert Polk
// Copyright (c) 2024 BLINK. All rights reserved.
// Last Modified: 11/28/2024

#include "control/extendedQuaternion.h"

ExtendedQuaternion::ExtendedQuaternion() : Quaternion() {}

ExtendedQuaternion::ExtendedQuaternion(float w, float x, float y, float z) : Quaternion(w, x, y,
                                                                                        z) {}

ExtendedQuaternion ExtendedQuaternion::operator-() const {
    return {-w, -x, -y, -z};
}

float ExtendedQuaternion::dot(const Quaternion &rhs) const {
    return (w * rhs.w) + (x * rhs.x) + (y * rhs.y) + (z * rhs.z);
}

float ExtendedQuaternion::dot(const ExtendedQuaternion &rhs) const {
    return (w * rhs.w) + (x * rhs.x) + (y * rhs.y) + (z * rhs.z);
}