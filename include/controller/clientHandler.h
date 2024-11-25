// Author: Robert Polk
// Copyright (c) 2024 BLINK. All rights reserved.
// Last Modified: 11/24/2024

#ifndef CLIENTHANDLER_H
#define CLIENTHANDLER_H

#include <Arduino.h>
#include <ArduinoLog.h>
#include <NimBLEDevice.h>

struct ClientCallbacks final : public NimBLEClientCallbacks {
    void onConnect(NimBLEClient *connectedClient) override;

    void onDisconnect(NimBLEClient *disconnectedClient, int reason) override;
};

struct ScanCallbacks final : public NimBLEScanCallbacks {
    void onResult(NimBLEAdvertisedDevice *advertisedDevice) override;

    void onScanEnd(NimBLEScanResults results) override;
};

static void notifyCallback(NimBLERemoteCharacteristic* pRemoteCharacteristic, uint8_t* pData,
                           size_t length, bool isNotify);

/**
 *
 */
class ClientHandler {
public:
    // Delete copy-constructor and assignment-op
    ClientHandler(const ClientHandler &) = delete;

    ClientHandler &operator=(const ClientHandler &) = delete;

    // Destructor
    ~ClientHandler();

    void initialize(const std::string &SERVICE_UUID_TO_SET, const std::string
    &IMU_CHARACTERISTIC_UUID_TO_SET, const std::string &DEVICE_NAME, const uint8_t
    &SCAN_TIME_TO_SET, const uint32_t &SCAN_WINDOW, const uint32_t &SCAN_INTERVAL);

    static ClientHandler *instance();

    [[noreturn]] void loop();

    // Public Member variables - used by callbacks
    static NimBLEAdvertisedDevice *advDevice;
    static std::string SERVICE_UUID;
    static uint8_t SCAN_TIME;
    static bool doConnect;  // If the client should try to connect to a device

private:
    ClientHandler() = default;

    bool connectToServer();

    // Member Variables
    static ClientHandler *inst; // Ptr to the singleton inst
    static ClientCallbacks clientCallback; // Client callback instance
    static ScanCallbacks scanCallback; // Scan callback instance
    static bool initialized;    // Initialization flag
    std::string IMU_CHARACTERISTIC_UUID;

};

#endif // CLIENTHANDLER_H