// Author: Robert Polk
// Copyright (c) 2024 BLINK. All rights reserved.
// Last Modified: 01/30/25

//================================================================================================//

/*
 * Welcome to the inner workings of BLINK's eyeball mechanism - the brain to the eye if you will.
 * This is the main program which first configures the program for logging and to interface with
 * the physical components of the mechanism.
 *
 * To configure this program to interface with each of the components in the eyeball mechanism,
 * read through the following sections and set the variables accordingly. Each section configures
 * a different component of the program and outlines configuration and program variables. Only edit
 * the configuration variables. The sections are in the following order:
 *      Logging
 *      BLE Client
 *      Encoders
 *      Motors
 */

//================================================================================================//

// #include the necessary header files - Do not edit
#include <Arduino.h>
#include <ArduinoLog.h>
#include <array>
#include "mechanism/clientHandler.h"
#include "mechanism/encoderHandler.h"
#include "mechanism/motorHandler.h"

/*
 * Logging
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
 * Uncomment one of the lines below to select the desired logging level. To remove logging for
 * certain files go into their .h files and uncomment '#define DISABLE_LOGGING'. To completely
 * remove logging, go into the ArduinoLog.h file and uncomment line 38 to define DISABLE_LOGGING.
 *
 * Set the baud rate for serial communication. This should match the value in the platformio.ini
 * file.
 */

// Configuration Variables
// constexpr uint8_t LOG_LEVEL = LOG_LEVEL_SILENT;   // The level of messages to show. Uncomment
// constexpr uint8_t LOG_LEVEL = LOG_LEVEL_FATAL;    // the desired level
// constexpr uint8_t LOG_LEVEL = LOG_LEVEL_ERROR;
// constexpr uint8_t LOG_LEVEL = LOG_LEVEL_WARNING;
// constexpr uint8_t LOG_LEVEL = LOG_LEVEL_NOTICE;
// constexpr uint8_t LOG_LEVEL = LOG_LEVEL_TRACE;
constexpr uint8_t LOG_LEVEL = LOG_LEVEL_VERBOSE;
constexpr uint32_t BAUD_RATE = 115200;  // The baud rate for serial communication

/*
 * Configure BLE Client
 *
 * This section configures the BLE Client by setting the UUIDs and device name. The UUIDs need to
 * match those set in server/server.cpp in order for the client to connect properly. New UUIDs
 * can be generated at https://www.uuidgenerator.net/
 */

// Configuration Variables
const std::string SERVICE_UUID =
        "da2aa210-e2ab-4d96-8d94-8536ec5a2728"; // The UUID for the service the client should
// connect to
const std::string IMU_CHARACTERISTIC_UUID =
        "72b9a4be-85fe-4cd5-ae42-f32414542c5a"; // The UUID for the IMU characteristic
const std::string DEVICE_NAME = "Controller";   // The name of the device that the client is on
constexpr uint8_t SCAN_TIME = 0;        // The duration of a scan in ms (0 is indefinite)
constexpr uint32_t SCAN_WINDOW = 15;    // The scan window in ms
constexpr uint32_t SCAN_INTERVAL = 45;  // The scan interval in ms

// Program Variables
TaskHandle_t clientLoopHandle = nullptr;    // Ptr to the client's FreeRTOS task
std::array <float, 4> quaternion; // Quaternion container : w, x, y, z

/*
 * Encoders
 *
 * This section configures the encoders attached to the Pololu 34:1 motor which tracks the
 * shafts' rotation. Set the following pin variables to the corresponding GPIO pins on the ESP32.
 * The yellow wires correspond to Channel A, and white wires Channel B.
 */

// Configuration Variables
constexpr uint8_t FIRST_ENCODER_PIN_A = 0; // The Channel A pin for the first encoder
constexpr uint8_t FIRST_ENCODER_PIN_B = 0; // The Channel B pin for the first encoder
constexpr uint8_t SECOND_ENCODER_PIN_A = 0;
constexpr uint8_t SECOND_ENCODER_PIN_B = 0;
constexpr uint8_t THIRD_ENCODER_PIN_A = 18;
constexpr uint8_t THIRD_ENCODER_PIN_B = 19;

// Program Variables
constexpr std::array<std::array<uint8_t, 2>, 3> encoderPins = {FIRST_ENCODER_PIN_A,
                                                               FIRST_ENCODER_PIN_B,
                                                               SECOND_ENCODER_PIN_A,
                                                               SECOND_ENCODER_PIN_B,
                                                               THIRD_ENCODER_PIN_A,
                                                               THIRD_ENCODER_PIN_B};
TaskHandle_t encoderLoopHandle = nullptr;   // Ptr to the encoder's FreeRTOS task
std::array <int64_t, 3> counts;    // Encoder count container : motorA, motorB, motorC

/*
 * Configure Motors
 *
 * This section configures the motors with the DRV8871 motor drivers. Set the following pin
 * variables to the corresponding GPIO pins on the ESP32. Set the PWM frequency and resolution
 */

// Configuration Variables
constexpr uint8_t FIRST_DRIVER_PWM_PIN = 0; // GPIO pin connected to IN1
constexpr uint8_t FIRST_DRIVER_DIRECTION_PIN = 0; // GPIO pin connected to IN2
constexpr uint8_t SECOND_DRIVER_PWM_PIN = 0;
constexpr uint8_t SECOND_DRIVER_DIRECTION_PIN = 0;
constexpr uint8_t THIRD_DRIVER_PWM_PIN = 0;
constexpr uint8_t THIRD_DRIVER_DIRECTION_PIN = 0;
constexpr uint32_t PWM_FREQUENCY = 20000;   // The frequency of the PWM signal
constexpr uint8_t PWM_RESOLUTION = 8;   // The resolution of the PWM duty cycle in bits

// Program Variables
constexpr std::array<std::array<uint8_t, 2>, 3> motorPins = {FIRST_DRIVER_DIRECTION_PIN,
                                                             FIRST_DRIVER_PWM_PIN,
                                                             SECOND_DRIVER_DIRECTION_PIN,
                                                             SECOND_DRIVER_PWM_PIN,
                                                             THIRD_DRIVER_DIRECTION_PIN,
                                                             THIRD_DRIVER_PWM_PIN};
TaskHandle_t motorTaskHandle = nullptr;   // Ptr to the MotorHandler's FreeRTOS task

//================================================================================================//

/**
 * Restart the ESP32
 */
void restart() {
    Log.fatalln("Fatal error occurred. Restarting the ESP32");
    ESP.restart();
}

//todo make a class to handle all of these background tasks
/**
 * A freeRTOS task for the ClientHandler loop
 *
 * @param param - Any parameters to be used by the task (none)
 */
void clientLoopTask(void *param) {
    Log.infoln("Starting ClientHandler loop");
    ClientHandler::instance()->loop();
}

/**
 * A freeRTOS task for the EncoderHandler loop
 *
 * @param param - Any parameters to be used by the task (none)
 */
void encoderLoopTask(void *param) {
    Log.infoln("Starting EncoderHandler loop");
    EncoderHandler::instance()->loop();
}

/**
 * A freeRTOS task for the MotorHandler
 *
 * @param param - Any parameters to be used by the task (none)
 */
void motorHandlerTask(void *param) {
    Log.infoln("Starting MotorHandler task");
    ClientHandler::instance()->loop();
}

void setup() {
    // Establish serial and logging
    Serial.begin(BAUD_RATE);
    Log.begin(LOG_LEVEL, &Serial, true);
    Log.infoln("Serial and logging initialized");

    // Initialize the BLE Client
    try {
        ClientHandler::instance()->initialize(SERVICE_UUID, IMU_CHARACTERISTIC_UUID, DEVICE_NAME,
                                              SCAN_TIME, SCAN_WINDOW, SCAN_INTERVAL);
    } catch (const std::exception &ex) {
        Log.errorln("Failed to initialize ClientHandler - %s", ex.what());
        restart();
    } catch (...) {
        Log.errorln("Failed to initialize ClientHandler - Unknown Error");
        restart();
    }

    // Create background task for the client
    BaseType_t clientResult = xTaskCreate(clientLoopTask, "ClientHandler::Loop",
                                          2048, nullptr, 1, &clientLoopHandle);

    if (clientResult != pdPASS) {
        Log.errorln("Failed to create clientLoopTask");
        restart();
    }

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
                                           2048, nullptr, 2, &encoderLoopHandle);

    if (encoderResult != pdPASS) {
        Log.errorln("Failed to create encoderLoopTask");
        restart();
    }

    // Initialize the MotorHandler
    try {
        MotorHandler::instance()->initialize(motorPins, PWM_FREQUENCY, PWM_RESOLUTION);
    } catch (const std::exception &ex) {
        Log.errorln("Failed to initialize MotorHandler - %s", ex.what());
    } catch (...) {
        Log.errorln("Failed to initialize MotorHandler - Unknown Error");
    }

    // Create background task for the motors
    BaseType_t motorResult = xTaskCreate(motorHandlerTask, "MotorHandler::Loop",
                                          2048, nullptr, 3, &motorTaskHandle);

    if (motorResult != pdPASS) {
        Log.errorln("Failed to create MotorHandlerTask");
        restart();
    }


    // Prompt user for first command
    Serial.print("Enter a command or enter 'h' for help: ");
}

/**
 * This is the main loop for the program. It parses serial commands and sends them to the
 * MotorHandler
 */
void loop() {
    if (Serial.available() > 0) {
        char command = Serial.read();

        if (command == '\n' || command == '\r') {
            return; // ???
        }

        // Execute command
        if (command == 'h') {
            MotorHandler::help();
        } else if (command == 'x') {
            if (motorTaskHandle != nullptr) {
                vTaskDelete(motorTaskHandle);
                motorTaskHandle = nullptr;
            }
            restart();
        } else if(command == 'f') {
            MotorHandler::instance()->forward();
        } else if (command == 'b') {
            MotorHandler::instance()->backward();
        } else if (command == 's') {
            MotorHandler::instance()->stop();
        } else if (command == 'l') {
            MotorHandler::instance()->moveLoop();
        } else if (command == 't') {
            MotorHandler::instance()->test();
        } else {
            Serial.println("Command not recognized");
        }

        Serial.print("Enter a command: ");
    }

    // Send data over serial to ROS2
    quaternion = ClientHandler::instance()->getQuaternion();
    counts = EncoderHandler::instance()->getCounts();

    Serial.printf("Q:\t%f\t%f\t%f\t%f\n", quaternion[0], quaternion[1], quaternion[2],
                   quaternion[3]);
    Serial.printf("E:\t%lld\t%lld\t%lld\n", counts[0], counts[1], counts[2]);
}