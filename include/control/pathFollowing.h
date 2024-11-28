// Author: Robert Polk
// Copyright (c) 2024 BLINK. All rights reserved.
// Last Modified: 11/27/2024

#ifndef PATHFOLLOWING_H
#define PATHFOLLOWING_H

#include <Arduino.h>
#include "controlAlgoImpl.h"

/**
 * This class defines a control algorithm that follows a predetermined path
 */
class PathFollowing final : public ControlAlgoImpl {
public:
    // Delete copy-constructor and assignment-op
    PathFollowing(const PathFollowing &) = delete;

    PathFollowing &operator=(const PathFollowing &) = delete;

    // Default destructor
    ~PathFollowing() override = default;

    friend class Factory;   // For construction
private:
    /**
     * Primary constructor - used by factory
     */
    PathFollowing();

    ExtendedQuaternion setTargetQuaternion() override;
};

#endif // PATHFOLLOWING_H