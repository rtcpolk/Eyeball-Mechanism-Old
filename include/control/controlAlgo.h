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
    ControlAlgo(const ControlAlgo&) = delete;
    ControlAlgo &operator=(const ControlAlgo&) = delete;

    // Default destructor
    ~ControlAlgo() = default;

    /**
     * Execute the control algo
     * @return True if successful
     */
    bool execute() const;

    friend class Factory; // For construction
private:
    /**
     * Primary constructor
     *
     * @param impl - Rhs of the control algo bridge
     */
    explicit ControlAlgo(ControlAlgoImpl *impl);

    // Member Variables
    std::shared_ptr<ControlAlgoImpl> bridge;  // Ptr to rhs of the control algo bridge
};

#endif // CONTROLALGO_H