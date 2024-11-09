// Author: Robert Polk
// Copyright (c) 2024 BLINK. All rights reserved.
// Last Modified: 11/07/24

//================================================================================================//

/* Welcome to the inner workings of BLINK's eyeball mechanism - the brain to the eye if you will.
 * This is the main program that first configures the program for logging and to interface with
 * the physical components of the mechanism. It then runs the general control loop. The control
 * loop...
 *
 * To properly configure this program for the physical eyeball mechanism setup, read through the
 * following sections and set the variables accordingly. The first portion defines macros that
 * control SerialLogging. The second portion #includes all necessary files, don't edit these. The
 * third portion defines pin connections and other parameters for each of the components in the
 * mechanism
 */

//================================================================================================//

/**
 * @brief Serial Logging
 *
 * The macros below enable different log messages to be printed to the Serial. Select the level
 * of logging by uncommenting the corresponding macro. Then select the files you wish to get log
 * messages from. Set the baudRate to match that of the platformio.ini file. Provide the logging
 * tag for main.cpp
 *
 * @param BAUD_RATE - The baudRate for Serial communication
 * @param mainTag - The logging tag for main.cpp
 */

constexpr int BAUD_RATE = 115200;

/**
 * @breif #includes
 *
 * The following files are used throughout main.cpp
 */
#include <Arduino.h>
#include "../../lib/Arduino-Log/ArduinoLog.h"
#include "actuator/clientHandler.h"

/**
 * @brief Configure the BLE Client - ESP32
 *
 * The variables below provide the UUIDs for the BLE client. These need to match the UUIDs set in
 * *'server.h* otherwise the client will not be able to connect to the server
 * properly. New UUIDs can be generated at https://www.uuidgenerator.net/
 *
 * @param SERVICE_UUID - The UUID for the service the client should connect to
 * @param IMU_CHARACTERISTIC_UUID - The UUID for the IMU characteristic
 * @param CLIENT_NAME - The name of the BLE Client
 *
 * There are also three class constants that control scanning events. These can be adjusted in the
 * clientBLE.h file.
 */
const std::string SERVICE_UUID = "da2aa210-e2ab-4d96-8d94-8536ec5a2728";
const std::string IMU_CHARACTERISTIC_UUID = "72b9a4be-85fe-4cd5-ae42-f32414542c5a";
const std::string DEVICE_NAME = "Eyeball Controller";

//================================================================================================//

// What goes here?

//================================================================================================//

void setup() {
    Serial.begin(BAUD_RATE);
    delay(500);
    Log.begin(LOG_LEVEL_VERBOSE, &Serial, true);
    ClientHandler::initialize(SERVICE_UUID, IMU_CHARACTERISTIC_UUID, DEVICE_NAME);
}

void loop() {
}