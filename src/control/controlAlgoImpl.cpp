// Author: Robert Polk
// Copyright (c) 2024 BLINK. All rights reserved.
// Last Modified: 11/26/2024

#include "control/controlAlgoImpl.h"

bool ControlAlgoImpl::execute() {
    getTarget();
    return true;
}