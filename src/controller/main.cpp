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

// #include the necessary header files - don't change these
#include <Arduino.h>
#include <ArduinoLog.h>
#include "controller/actuator/clientHandler.h"

/*
 * Configure Logging
 *
 * This section determines the level of logging within the program. The messages are outputted
 * to the Serial monitor. The following log levels are available:
 *      0 - LOG_LEVEL_SILENT     no output
 *      1 - LOG_LEVEL_FATAL      fatal errors
 *      2 - LOG_LEVEL_ERROR      all errors
 *      3 - LOG_LEVEL_WARNING    errors, and warnings
 *      4 - LOG_LEVEL_NOTICE     errors, warnings and notices
 *      5 - LOG_LEVEL_TRACE      errors, warnings, notices & traces
 *      6 - LOG_LEVEL_VERBOSE    all
 * Uncomment one of the lines below to select the desired logging level. To completely remove
 * logging, go into the ArduinoLog.h file and uncomment line 38 to define DISABLE_LOGGING.
 *
 * Set the baud rate for serial communication. This should match the value in the platformio.ini
 * file.
 * 
 * Variables:
 *  LOG_LEVEL - The level of messages to show. Uncomment the desired level
 *  BAUD_RATE - The baud rate for serial communication
 */

//constexpr uint8_t LOG_LEVEL = LOG_LEVEL_SILENT;
// constexpr uint8_t LOG_LEVEL = LOG_LEVEL_FATAL;
// constexpr uint8_t LOG_LEVEL = LOG_LEVEL_ERROR;
// constexpr uint8_t LOG_LEVEL = LOG_LEVEL_WARNING;
//constexpr uint8_t LOG_LEVEL = LOG_LEVEL_NOTICE;
constexpr uint8_t LOG_LEVEL = LOG_LEVEL_TRACE;
//constexpr uint8_t LOG_LEVEL = LOG_LEVEL_VERBOSE;
constexpr uint32_t BAUD_RATE = 115200;

/*
 * Configure BLE Client
 *
 * This section configures the BLE Client by setting the UUIDs and device name. The UUIDs need to
 * match those set in server.cpp in order for the client to connect properly. New UUIDs can be
 * generated at https://www.uuidgenerator.net/
 *
 * In addition to the variables below, there are three class constants that control scanning
 * events. Those can be adjusted in the clientHandler.h file if need be.
 *
 * Variables:
 *  SERVICE_UUID - The UUID for the service the client should connect to
 *  IMU_CHARACTERISTIC_UUID - The UUID for the IMU characteristic
 *  DEVICE_NAME - The name of the device that the client is on
 */

const std::string SERVICE_UUID = "da2aa210-e2ab-4d96-8d94-8536ec5a2728";
const std::string IMU_CHARACTERISTIC_UUID = "72b9a4be-85fe-4cd5-ae42-f32414542c5a";
const std::string DEVICE_NAME = ""; // It seems to not connect when !empty




//================================================================================================//

/**
 * Restart the ESP32
 */
void restart() {
    Log.fatalln("Fatal error occurred. Restarting the ESP32");
    ESP.restart();
}

void setup() {
    // Establish serial and logging
    Serial.begin(BAUD_RATE);
    Log.begin(LOG_LEVEL, &Serial, true);
    Log.infoln("Serial and logging initialized");

    // Initialize the BLE Client
    Log.infoln("Attempting to initialize ClientHandler");
    try {
        bool initialized = ClientHandler::initialize(SERVICE_UUID, IMU_CHARACTERISTIC_UUID,
                                                 DEVICE_NAME);

        if (!initialized) {
            restart();
        }

    } catch (const std::exception& ex) {
        Log.errorln("ClientHandler::initialize - Exception caught: %s", ex.what());
        restart();
    } catch (...) {
        Log.errorln("ClientHandler::initialize - Unknown Exception");
        restart();
    }
}

void loop() {
}