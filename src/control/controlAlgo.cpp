// Author: Robert Polk
// Copyright (c) 2024 BLINK. All rights reserved.
// Last Modified: 11/25/2024

#include "control/controlAlgo.h"

bool ControlAlgo::execute() const { return bridge->execute(); }

ControlAlgo::ControlAlgo(ControlAlgoImpl *impl) : bridge(impl) {}