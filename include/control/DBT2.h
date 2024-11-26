// Author: Robert Polk
// Copyright (c) 2024 BLINK. All rights reserved.
// Last Modified: 11/25/2024

#ifndef DBT2_H
#define DBT2_H

#include <Arduino.h>
#include"controlAlgoImpl.h"

class DBT2 final : public ControlAlgoImpl {
public:
    // Default copy-constructor, assignment-op, and destructor
    DBT2(const DBT2&) = default;
    DBT2 &operator=(const DBT2&) = default;
    ~DBT2() = default;

    friend class Factory;
private:
    // Only allow factory to create
    DBT2() = default;

    Quaternion getTarget();
};

#endif // DBT2_H