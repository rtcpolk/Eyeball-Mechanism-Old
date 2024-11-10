// Author: Robert Polk
// Copyright (c) 2024 BLINK. All rights reserved.
// Last Modified: 11/09/24

//================================================================================================//

/*
 * Welcome to the inner workings of BLINK's eyeball mechanism - the brain to the eye if you will.
 * This is the main program that first configures the program for logging and to interface with
 * the physical components of the mechanism. It then runs the general control loop. The control
 * loop...
 *
 * To properly configure this program to run with each of the components in the eyeball
 * mechanism, read through the following sections and set the variables accordingly. These
 * sections set pin connections and other parameters to interface with the physical setup. The
 * variables are explained in the comments above each section. The sections are in the following
 * order:
 *      Logging
 *      BLE Client
 */

//================================================================================================//

// #include the necessary header files, don't change these
#include <Arduino.h>
#include <ArduinoLog.h>
#include "controller/actuator/clientHandler.h"

/*
 * Configure Logging
 *
 * This section determines the level of logging within the program. The messages are outputted
 * to the Serial monitor. THe following log levels are available:
 *      0 - LOG_LEVEL_SILENT     no output
 *      1 - LOG_LEVEL_FATAL      fatal errors
 *      2 - LOG_LEVEL_ERROR      all errors
 *      3 - LOG_LEVEL_WARNING    errors, and warnings
 *      4 - LOG_LEVEL_NOTICE     errors, warnings and notices
 *      5 - LOG_LEVEL_TRACE      errors, warnings, notices & traces
 *      6 - LOG_LEVEL_VERBOSE    all
 * To completely remove logging, go into the ArduinoLog.h file and uncomment line 38 to define
 * DISABLE_LOGGING.
 * 
 * LOG_LEVEL - The level of messages to show. Uncomment the desired level
 * BAUD_RATE - Set the baud rate for serial communication. This should match the value in the
 *             platformio.ini file
 */

//constexpr uint8_t LOG_LEVEL = LOG_LEVEL_SILENT;
// constexpr uint8_t LOG_LEVEL = LOG_LEVEL_FATAL;
// constexpr uint8_t LOG_LEVEL = LOG_LEVEL_ERROR;
// constexpr uint8_t LOG_LEVEL = LOG_LEVEL_WARNING;
//constexpr uint8_t LOG_LEVEL = LOG_LEVEL_NOTICE;
//constexpr uint8_t LOG_LEVEL = LOG_LEVEL_TRACE;
constexpr uint8_t LOG_LEVEL = LOG_LEVEL_VERBOSE;
constexpr uint32_t BAUD_RATE = 115200;

/*
 * Configure BLE Client
 */
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

//================================================================================================//

void setup() {
    Serial.begin(BAUD_RATE);
    Log.begin(LOG_LEVEL, &Serial, true);

    ClientHandler::initialize(SERVICE_UUID, IMU_CHARACTERISTIC_UUID, DEVICE_NAME);
}

void loop() {
}