// Author: Robert Polk
// Copyright (c) 2024 BLINK. All rights reserved.
// Last Modified: 11/25/2024

#include "control/DBT2.h"

DBT2::DBT2() : ControlAlgoImpl() {
    Log.traceln("dbt2 Created");
}

Quaternion DBT2::setTargetQuaternion() {
    //todo update
    Log.traceln("dbt2 target set");
    return Quaternion();
}

void DBT2::PID() {
    Log.traceln("executing DBT2 PID");
    std::array<int16_t, 3> speeds;

    // First run each motor forward individually
    speeds = {1, 0, 0};
    MotorHandler::instance()->setMotorSpeeds(speeds);
    delay(1000);

    speeds = {0, 1, 0};
    MotorHandler::instance()->setMotorSpeeds(speeds);
    delay(1000);

    speeds = {0, 0, 1};
    MotorHandler::instance()->setMotorSpeeds(speeds);
    delay(1000);

    speeds = {0, 0, 0};
    MotorHandler::instance()->setMotorSpeeds(speeds);
    delay(1000);

    // Second run each motor backward individually
    speeds = {1, 0, 0};
    MotorHandler::instance()->setMotorSpeeds(speeds);
    delay(1000);

    speeds = {0, 1, 0};
    MotorHandler::instance()->setMotorSpeeds(speeds);
    delay(1000);

    speeds = {0, 0, 1};
    MotorHandler::instance()->setMotorSpeeds(speeds);
    delay(1000);

    speeds = {0, 0, 0};
    MotorHandler::instance()->setMotorSpeeds(speeds);
    delay(1000);

    // Third experiment with pairs of motors
    speeds = {1, 1, 0};
    MotorHandler::instance()->setMotorSpeeds(speeds);
    delay(1000);

    speeds = {0, 1, 1};
    MotorHandler::instance()->setMotorSpeeds(speeds);
    delay(1000);

    speeds = {1, 0, 1};
    MotorHandler::instance()->setMotorSpeeds(speeds);
    delay(1000);

    speeds = {0, 0, 0};
    MotorHandler::instance()->setMotorSpeeds(speeds);
}