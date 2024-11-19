// Author: Robert Polk
// Copyright (c) 2024 BLINK. All rights reserved.
// Last Modified: 11/19/24

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
 *      Encoders
 */

//================================================================================================//

// #include the necessary header files - Do not edit
#include <Arduino.h>
#include <ArduinoLog.h>
#include "controller/clientHandler.h"
#include "controller/encoderHandler.h"

/*
 * Configure Logging
 *
 * This section determines the level of logging within the program. The messages are written to
 * the Serial monitor. The following log levels are available:
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
 *   LOG_LEVEL - The level of messages to show. Uncomment the desired level
 *   BAUD_RATE - The baud rate for serial communication
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
 * match those set in server/server.cpp in order for the client to connect properly. New UUIDs
 * can be generated at https://www.uuidgenerator.net/
 *
 * In addition to the variables below, there are three class constants that control scanning
 * events. They do not need to be adjusted however they can be found in the clientHandler.h file if
 * further optimization is to be done.
 *
 * Variables:
 *   SERVICE_UUID - The UUID for the service the client should connect to
 *   IMU_CHARACTERISTIC_UUID - The UUID for the IMU characteristic
 *   DEVICE_NAME - The name of the device that the client is on
 */

const std::string SERVICE_UUID = "da2aa210-e2ab-4d96-8d94-8536ec5a2728";
const std::string IMU_CHARACTERISTIC_UUID = "72b9a4be-85fe-4cd5-ae42-f32414542c5a";
const std::string DEVICE_NAME = ""; // It seems to not connect when !empty

/*
 * Configure Encoders
 *
 * This section configures the encoders that track the motor shafts' rotation. Set the following
 * pin variables to the corresponding GPIO pins on the ESP32. Do not edit the encoderPins variable
 *
 * Variables:
 *   #_ENCODER_PIN_A - The Channel A pin for the # encoder. Corresponds to the yellow wire
 *   #_ENCODER_PIN_B - The Channel B pin for the # encoder. Corresponds to the white wire
 *   encoderPins - An array that holds all the encoder pins. Do not edit
 */
constexpr uint8_t FIRST_ENCODER_PIN_A = 15;
constexpr uint8_t FIRST_ENCODER_PIN_B = 16;
constexpr uint8_t SECOND_ENCODER_PIN_A = 17;
constexpr uint8_t SECOND_ENCODER_PIN_B = 18;
constexpr uint8_t THIRD_ENCODER_PIN_A = 19;
constexpr uint8_t THIRD_ENCODER_PIN_B = 20;
constexpr std::array<std::array<uint8_t, 2>, 3> encoderPins = {FIRST_ENCODER_PIN_A,
                                                               FIRST_ENCODER_PIN_B,
                                                               SECOND_ENCODER_PIN_A,
                                                               SECOND_ENCODER_PIN_B,
                                                               THIRD_ENCODER_PIN_A,
                                                               THIRD_ENCODER_PIN_B};

//================================================================================================//

TaskHandle_t encoderLoopHandle = nullptr;
TaskHandle_t clientLoopHandle = nullptr;
int count;

/**
 * Restart the ESP32
 */
void restart() {
    Log.fatalln("Fatal error occurred. Restarting the ESP32");
    ESP.restart();
}

/**
 * A task for the EncoderHandler loop to be run in the background
 *
 * @param param - Any parameters to be used by the task (none)
 */
void encoderLoopTask(void *param) {
    Log.infoln("Starting EncoderHandler loop");
    EncoderHandler::instance()->loop();
}

/**
 * A task for the ClientHandler loop to be run in the background
 *
 * @param param - Any parameters to be used by the task (none)
 */
void clientLoopTask(void *param) {
    Log.infoln("Starting ClientHandler loop");
    ClientHandler::instance()->loop();
}

void setup() {
    // Establish serial and logging
    Serial.begin(BAUD_RATE);
    Log.begin(LOG_LEVEL, &Serial, true);
    Log.infoln("Serial and logging initialized");

    // Initialize the EncoderHandler
    try {
        EncoderHandler::instance()->initialize(encoderPins);
    } catch (const std::exception &ex) {
        Log.errorln("Failed to initialize EncoderHandler - %s", ex.what());
        restart();
    } catch (...) {
        Log.errorln("Failed to initialize EncoderHandler - Unknown Error");
        restart();
    }

    // Create background task for the encoder
    BaseType_t encoderResult = xTaskCreate(encoderLoopTask, "EncoderHandler::Loop",
                                    2048, nullptr, 1, &encoderLoopHandle);

    if (encoderResult != pdPASS) {
        Log.errorln("Failed to create encoderLoopTask");
        restart();
    }

    // Initialize the BLE Client
    try {
        count = 0; //todo update this with client when ready
    } catch (const std::exception &ex) {
        Log.errorln("Failed to initialize ClientHandler - %s", ex.what());
        restart();
    } catch (...) {
        Log.errorln("Failed to initialize ClientHandler - Unknown Error");
        restart();
    }

    // Create background task for the client
    BaseType_t clientResult = xTaskCreate(clientLoopTask, "EncoderHandler::Loop",
                                    2048, nullptr, 1, &encoderLoopHandle);

    if (clientResult != pdPASS) {
        Log.errorln("Failed to create clientLoopTask");
        restart();
    }
}

void loop() {
    if (count == 3) {
        Log.infoln("Suspending encoder loop");
        vTaskSuspend(encoderLoopHandle);
    }

    if (count == 6) {
        Log.infoln("Resuming encoder loop");
        vTaskResume(encoderLoopHandle);
    }

    if (count == 9) {
        Log.infoln("Suspending encoder loop");
        vTaskSuspend(encoderLoopHandle);
    }

    if (count == 12) {
        Log.infoln("Resuming encoder loop");
        vTaskResume(encoderLoopHandle);
    }

    Log.infoln("I'm looping");

    ++count;
    delay(1000);
}