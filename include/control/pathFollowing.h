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
    // Default copy-constructor, assignment-op, and destructor
    PathFollowing(const PathFollowing &) = default;

    PathFollowing &operator=(const PathFollowing &) = default;

    ~PathFollowing() override = default;

    friend class Factory;   // For construction
private:
    PathFollowing();

    Quaternion getTarget() override;
};

#endif // PATHFOLLOWING_H