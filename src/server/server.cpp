// Author: Robert Polk
// Copyright (c) 2024 BLINK. All rights reserved.
// Last Modified: 11/19/2024

#include <Arduino.h>
#include <ArduinoLog.h>
#include <NimBLEDevice.h>
#include "C:\Users\rober\blink\lib\I2Cdev\I2Cdev.h"
#include "C:\Users\rober\blink\lib\MPU6050\MPU6050_6Axis_MotionApps20.h"

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

//todo update below for server
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
const std::string DEVICE_NAME = "Eyeball"; // It seems to not connect when !empty


static NimBLEServer *server;


struct ServerCallbacks final : public NimBLEServerCallbacks {};
struct CharacteristicCallbacks final: public NimBLECharacteristicCallbacks {};
struct DescriptorCallbacks final : public NimBLEDescriptorCallbacks {};

static ServerCallbacks serverCallback;
static CharacteristicCallbacks characteristicCallback;
static DescriptorCallbacks descriptorCallback;

void restart() {
    Log.fatalln("Fatal error occurred. Restarting the ESP32");
    ESP.restart();
}

void setupBLEServer() {
    // Initialize BLE Device
    NimBLEDevice::init(DEVICE_NAME);

    // Create the server
    server = NimBLEDevice::createServer();
    server->setCallbacks(&serverCallback);

    // Create service and characteristics
    NimBLEService *eyeballService = server->createService(SERVICE_UUID);
    NimBLECharacteristic *IMUCharacterisitc = eyeballService->createCharacteristic
    (IMU_CHARACTERISTIC_UUID, NIMBLE_PROPERTY::READ) | NIMBLE_PROPERTY::WRITE);
    IMUCharacterisitc->setCallbacks(&characteristicCallback);
}

void setupIMU() {}


void setup() {
    // Establish serial and logging
    Serial.begin(BAUD_RATE);
    Log.begin(LOG_LEVEL, &Serial, true);
    Log.infoln("Serial and logging initialized");

    // Set up the server
    try {
        setupBLEServer();
    } catch (const std::exception &ex) {
        Log.errorln("Failed to setup BLEServer - %s", ex.what());
        restart();
    } catch (...) {
        Log.errorln("Failed to setup BLEServer - Unknown Error");
        restart();
    }

    // Set up the IMU
    try {
        setupIMU();
    } catch (const std::exception &ex) {
        Log.errorln("Failed to setup IMU - %s", ex.what());
        restart();
    } catch (...) {
        Log.errorln("Failed to setup IMU - Unknown Error");
        restart();
    }
}

void loop() {
    if (clientConnected) {
        if (!DMPInit) {
            return;
        }
    }
}