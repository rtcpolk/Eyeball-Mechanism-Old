// Author: Robert Polk
// Copyright (c) 2024 BLINK. All rights reserved.
// Last Modified: 11/25/2024

#ifndef CLIENTHANDLER_H
#define CLIENTHANDLER_H

//#define DISABLE_LOGGING

#include <Arduino.h>
#include <ArduinoLog.h>
#include <NimBLEDevice.h>

/**
 * A struct to define what to do for client events
 */
struct ClientCallbacks final : public NimBLEClientCallbacks {
    /**
     * Called for connection events
     *
     * @param connectedClient - The client that connected
     */
    void onConnect(NimBLEClient *connectedClient) override;

    /**
     * Called for disconnection events. Starts a new scan
     *
     * @param disconnectedClient - The client that disconnected
     * @param reason - The reason for disconnect
     */
    void onDisconnect(NimBLEClient *disconnectedClient, int reason) override;
};

/**
 * A struct to define what to do for scan events
 */
struct ScanCallbacks final : public NimBLEScanCallbacks {
    /**
     * Called for each device found during a scan. Checks if it has the correct service UUID
     *
     * @param advertisedDevice - The device that was found
     */
    void onResult(NimBLEAdvertisedDevice *advertisedDevice) override;

    /**
     * Called when a scan ends
     *
     * @param results - The results of the scan
     */
    void onScanEnd(NimBLEScanResults results) override;
};

/**
 * A class to handle the BLE Client. It manages its connection to the server and is notified with
 * new data
 */
class ClientHandler {
public:
    // Delete copy-constructor and assignment-op
    ClientHandler(const ClientHandler &) = delete;

    ClientHandler &operator=(const ClientHandler &) = delete;

    // Destructor
    ~ClientHandler();

    /**
     * Initialize the Client Handler by creating a BLE Device and starting a scan
     *
     * @param SERVICE_UUID - The service UUID to look for
     * @param IMU_CHARACTERISTIC_UUID - The IMU Characteristic UUID to look for
     * @param DEVICE_NAME - The name of the client's BLE Device
     * @param SCAN_TIME - The duration of a scan in ms (0 is indefinite)
     * @param SCAN_WINDOW - The scan window in ms
     * @param SCAN_INTERVAL - The scan interval in ms
     */
    void initialize(const std::string &SERVICE_UUID, const std::string
    &IMU_CHARACTERISTIC_UUID, const std::string &DEVICE_NAME, const uint8_t
                    &SCAN_TIME, const uint32_t &SCAN_WINDOW, const uint32_t &SCAN_INTERVAL);

    /**
     * Get the singleton ClientHandler instance
     *
     * @return The instance ptr
     */
    static ClientHandler *instance();

    /**
     * Called when a subscribed characteristic notifies the client. It un-packages the IMU's
     * quaternion data
     *
     * @param remoteCharacteristic - The characteristic that notified the client
     * @param data - A ptr to the data received in the notification
     * @param length - The length of the data
     * @param isNotify - If the callback was triggered by a notification
     */
    static void notifyCallback(NimBLERemoteCharacteristic *remoteCharacteristic, uint8_t *data,
                               size_t length, bool isNotify);

    /**
     * Continuously manage the client's connection to the server
     */
    [[noreturn]] void loop();

    // Public Member variables - used by the callbacks
    static NimBLEAdvertisedDevice *advDevice;   // A ptr to a device with the correct UUID
    static std::string serviceUUID; // The service UUID to look for
    static uint32_t scanTime; // The duration of a scan in ms (0 is indefinite)
    static bool doConnect;  // If the client should try to connect to a device

private:
    /**
     * Primary Constructor
     */
    ClientHandler() = default;

    /**
     * Attempts to connect to a server with the correct service UUID. It then checks its
     * characteristic's properties and subscribes to notifications.
     *
     * @return True if successful
     */
    bool connectToServer();

    // Member Variables
    static ClientHandler *inst; // Ptr to the singleton inst
    static ClientCallbacks clientCallback; // Client callback instance
    static ScanCallbacks scanCallback; // Scan callback instance
    static bool initialized;    // Initialization flag
    static std::string IMUCharacteristicUUID;  // The IMU Characteristic UUID
};

#endif // CLIENTHANDLER_H