// Author: Robert Polk
// Copyright (c) 2024 BLINK. All rights reserved.
// Last Modified: 11/07/24

//================================================================================================//

/* Welcome to the inner workings of BLINK's eyeball mechanism - the brain to the eye if you will.
 * This is the main program that first sets up the program to interface with the physical
 * components and then runs the general control loop. The control loop...
 *
 * To properly configure this program for the physical eyeball mechanism setup, read through the
 * following sections and set the variables accordingly. The first subsection defines macros that
 * control logging. The second subsection #includes all necessary files, do not edit these. The
 * third subsection defines pin connections and other component specific parameters for each of the
 * components in the mechanism
 */

//================================================================================================//

#define LOGGING
#define LOGGING_DATA
#define LOGGING_DEBUG
#define LOGGING_INFO
#define LOGGING_WARNING
#define LOGGING_ERROR

#include <Arduino.h>
#include "actuator/clientHandler.h"

/**
 *  @brief Configure Logging
 *
 */
 constexpr int baudRate = 115200; //Has to math .ini
 static const char* mainTag = "main";


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

/**
 * @brief Main Variables
 *
 * The variables below are used throughout the main program
 */
//ClientHandler client(SERVICE_UUID, IMU_CHARACTERISTIC_UUID, DEVICE_NAME); // The client is
// created and
// member
// variables set

//================================================================================================//

void setup() {
    Serial.begin(baudRate);
delay (1000);
    Serial.println("Hello world!");
    esp_log_level_set("*", ESP_LOG_VERBOSE);
    delay(1000);

    ESP_LOGD(mainTag, "sample log");
    ESP_LOGI(mainTag, "test tes");
    ESP_LOGE(mainTag, "test sdfasdf");
    ESP_LOGW(mainTag, "test mow");
    ESP_LOGV(mainTag, "asdfnsdfj;jd");

    Serial.println("it should say smth");
}

void loop() {}