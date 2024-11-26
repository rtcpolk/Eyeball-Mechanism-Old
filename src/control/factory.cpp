// Author: Robert Polk
// Copyright (c) 2024 BLINK. All rights reserved.
// Last Modified: 11/25/2024

#include "control/factory.h"
#include "control/DBT2.h"

ControlAlgo Factory::makeControlAlgo() {
    return makeDBT2();
}

ControlAlgo Factory::makeDBT2() {
    return ControlAlgo(new DBT2());
}