// Author: Robert Polk
// Copyright (c) 2024 BLINK. All rights reserved.
// Last Modified: 11/25/2024

#ifndef DBT2_H
#define DBT2_H

#include <Arduino.h>
#include"controlAlgoImpl.h"

class DBT2 final : public ControlAlgoImpl {
public:
    // Delete copy-constructor and assignment-op
    DBT2(const DBT2 &) = delete;

    DBT2 &operator=(const DBT2 &) = delete;

    // Default destructor
    ~DBT2() override = default;

    friend class Factory;   // For construction
private:
    /**
     * Primary constructor - used by factory
     */
    DBT2();

    /**
     * Set the target quaternion - does nothing for dbt2
     * @return target quaternion
     */
    Quaternion setTargetQuaternion() override;

    /**
     * Arbitrarily control the motors to demonstrate movement capabilities. DBT2 does not
     * implement any actual feedback loop. The name just allows it to interface with the base
     * class for easy execution
     */
    void PID() override;
};

#endif // DBT2_H