// Author: Robert Polk
// Copyright (c) 2024 BLINK. All rights reserved.
// Last Modified: 11/25/2024

#include "control/controlAlgo.h"

ControlAlgo::~ControlAlgo() { delete bridge; }

bool ControlAlgo::execute() const { return bridge->execute(); }

ControlAlgo::ControlAlgo(ControlAlgoImpl *impl) : bridge(impl) {}


//bool ControlAlgo::execute() const { return bridge->execute(); }
//
//ControlAlgo::ControlAlgo(std::shared_ptr<ControlAlgoImpl> impl) : bridge(std::move(impl)) {}