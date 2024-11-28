// Author: Robert Polk
// Copyright (c) 2024 BLINK. All rights reserved.
// Last Modified: 11/25/2024

#ifndef CONTROLALGO_H
#define CONTROLALGO_H

#include <Arduino.h>
#include "controlAlgoImpl.h"
#include <memory>

/**
 * Lhs of the ControlAlgo bridge
 */
class ControlAlgo {
public:
    // Only allow factory to create
    ControlAlgo() = delete;

    // Allow move but not copy
    ControlAlgo(const ControlAlgo &) = delete;

    ControlAlgo &operator=(const ControlAlgo &) = delete;

    ControlAlgo(ControlAlgo &&) noexcept = default;

    ControlAlgo &operator=(ControlAlgo &&) noexcept = default;

    // Destructor
    ~ControlAlgo();

    /**
     * Execute the control algo
     */
    void execute() const;

    friend class Factory;   // For construction

private:
    /**
     * Primary constructor
     *
     * @param impl - Ptr to the rhs of the ControlAlgo bridge
     */
    explicit ControlAlgo(ControlAlgoImpl *impl);

    // Member Variables
    ControlAlgoImpl *bridge;  // Ptr to rhs of the ControlAlgo bridge
};

#endif  // CONTROLALGO_H