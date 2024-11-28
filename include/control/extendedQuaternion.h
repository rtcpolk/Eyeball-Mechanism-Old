// Author: Robert Polk
// Copyright (c) 2024 BLINK. All rights reserved.
// Last Modified: 11/28/2024

#ifndef EXTENDEDQUATERNION_H
#define EXTENDEDQUATERNION_H

#include <Arduino.h>
#include "../lib/MPU6050/helper_3dmath.h"

/**
 * A class to extend Quaternion with additional calculations
 */
class ExtendedQuaternion final : public Quaternion {
public:
    /**
     * Primary constructor - default values
     */
    ExtendedQuaternion();

    /**
     * Secondary constructor - known values
     *
     * @param w
     * @param x
     * @param y
     * @param z
     */
    ExtendedQuaternion(float w, float x, float y, float z);

    // Default copy-constructor and assignment-op
    ExtendedQuaternion(const ExtendedQuaternion &) = default;
    ExtendedQuaternion &operator=(const ExtendedQuaternion &) = default;

    // Operator overloads
    ExtendedQuaternion operator-() const;

    /**
     * Calculate the dot product between two quaternions
     *
     * @param rhs - The quaternion to take the dot product with
     * @return The dot product
     */
    float dot(const Quaternion &rhs) const;

    /**
    * Calculate the dot product between two quaternions
    *
    * @param rhs - The quaternion to take the dot product with
    * @return The dot product
    */
    float dot(const ExtendedQuaternion &rhs) const;
};

#endif // EXTENDEDQUATERNION_H