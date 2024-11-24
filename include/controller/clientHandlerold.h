// Author: Robert Polk
// Copyright (c) 2024 BLINK. All rights reserved.
// Last Modified: 11/07/24

#ifndef CLIENTHANDLER_H
#define CLIENTHANDLER_H

#include <memory>
#include "BLEDevice.h"
#include "ArduinoLog.h"

/**
 * This is a struct that implements the program's ClientCallback method. It inherits from the BLE
 * Library's abstract BLEClientCallbacks
 */
struct ClientCallbacks final : public BLEClientCallbacks {
    /**
     * This method defines what to do when a connection event occurs. It has no logic
     *
     * @param client - A ptr to the client
     */
    void onConnect(BLEClient *) override;

    /**
     * This method defines what to do when a disconnection event occurs. It sets
     * ClientHandler::connected to false
     *
     * @param client - A ptr to the client
     */
    void onDisconnect(BLEClient *) override;
};

/**
 * This is a struct that implements the program's AdvertisedDeviceCallback method. It inherits
 * from the BLE Library's abstract BLEAdvertisedDeviceCallbacks class
 */
struct AdvertisedDeviceCallbacks final : public BLEAdvertisedDeviceCallbacks {
    /**
     * This method defines what to do when a new device is found during a scan. After
     * scanner->start is called, the program checks for devices and this method is called
     * for every device that is discovered. It checks if the device has the service UUID that the
     * client is looking for. If true, it sets up a connection attempt
     *
     * @param advertisedDevice - The device that was found
     */
    void onResult(BLEAdvertisedDevice) override;
};

/**
 * A singleton class to handle everything related to the BLEClient
 */
class ClientHandler {
public:
    // Delete constructor, copy-constructor, and assignment-op
    ClientHandler() = delete;

    ClientHandler(const ClientHandler &) = delete;

    ClientHandler &operator=(const ClientHandler &) = delete;

    // Destructor
    ~ClientHandler();

    /**
     * Initialize the client by creating a BLE device and setting the scanning parameters
     *
     * @param SERVICE_UUID - The UUID of the service to connect to
     * @param IMU_CHARACTERISTIC_UUID - The UUID of the IMU characteristic
     * @param CLIENT_NAME - The name for the BLE device
     *
     * @return True if successful
     */
    static bool initialize(const std::string &, const std::string &, const
    std::string &);

    /**
     * Get the ClientHandler instance
     *
     * @return Ptr to the ClientHandler instance
     */
    static ClientHandler *instance();

    /**
     * Set the value of connected
     *
     * @param newConnected - The new value to set connected to
     */
    void setConnected(const bool &);

    /**
     * Set the value of server
     *
     * @param newServer - The new value to set server to
     */
    void setServer(BLEAdvertisedDevice *);

    /**
     * Set the value of attemptConnect
     *
     * @param newAttemptConnect - The new value to set attemptConnect to
     */
    void setAttemptConnect(const bool &);

    /**
     * Set the value of attemptScan
     *
     * @param newInitiateScan - The new value to set attemptScan to
     */
    void setInitiateScan(const bool &);

    /**
     * Get the serviceUUID object
     * @return The serviceUUID object
     */
    BLEUUID getServiceUUID() const;

    bool getAttemptConnect() const;

    bool getConnected() const;

    bool getInitiateScan() const;

    /**
     * The main loop that runs the client. Looks at if it should attempt to connect to a server
     * and if it is connected. This is an infinite loop
     */
    void loop();

    /**
 * Attempts to connect to a server. It checks if the server has the proper service and
 * characteristic UUIDs and tries to establish a connection
 *
 * @return True if successful
 */
    bool connectToServer();

private:
    /**
    * Private constructor for the singleton pattern
    *
    * @param SERVICE_UUID - The UUID of the service to connect to
    * @param IMU_CHARACTERISTIC_UUID - The UUID of the IMU characteristic
    * @param CLIENT_NAME - The name for the BLE device
    */
    ClientHandler(const std::string &, const std::string &, const std::string &);

    /**
     * Attempts to connect to a server. It checks if the server has the proper service and
     * characteristic UUIDs and tries to establish a connection
     *
     * @return True if successful
     */
   // bool connectToServer();

    /**
     * This method defines what to do when the characteristic sends a notification.
     *
     * @param characteristic - A ptr to the characteristic
     * @param data - A ptr to the data
     * @param length - The length of the data packet sent
     * @param xx - lksdfj
     */
    static void notifyCallback(BLERemoteCharacteristic *, uint8_t *, size_t, bool);

    // Member Variables
    static ClientHandler *inst; // Singleton ptr
    BLEUUID serviceUUID;           // The service UUID to connect to
    BLEUUID imuCharacteristicUUID; // The IMU's characteristic UUID
    bool attemptConnect; // If the client should attempt to connect to a server
    bool connected;      // If the client is currently connected
    bool initiateScan;   // If a new scan should be initiated after a disconnection
    BLEAdvertisedDevice *server; // Ptr to the remote server
    BLERemoteCharacteristic *IMUCharacteristic; // Ptr to the IMU's characteristic

    // Class Constants
    static constexpr int SCAN_INTERVAL = 40; // The interval at which scans are initiated, in ms
    static constexpr int SCAN_WINDOW = 30; // How long each individual scan is, in ms
    static constexpr int SCAN_DURATION = 5; // How long a scan event will run, in s
};

#endif // CLIENTHANDLER_H