// Author: Robert Polk
// Copyright (c) 2024 BLINK. All rights reserved.
// Last Modified: 

#include "actuator/ClientHandler.h"

ClientHandler::ClientHandler(const std::string &SERVICE_UUID,
                             const std::string &IMU_CHARACTERISTIC_UUID,
                             const std::string &CLIENT_NAME)
        : serviceUUID(SERVICE_UUID), imuCharacteristicUUID(IMU_CHARACTERISTIC_UUID),
          attemptConnect(false), connected(false), initiateScan(false), server
                  (nullptr), IMUCharacteristic(nullptr) {
    // Create the BLE device
    BLEDevice::init(CLIENT_NAME);
}

void ClientHandler::initialize() {
    // Retrieve a scanner and
    BLEScan *scanner = BLEDevice::getScan();
    scanner->setAdvertisedDeviceCallbacks(new AdvertisedDeviceCallback());
    scanner->setInterval(SCAN_INTERVAL);
    scanner->setWindow(SCAN_WINDOW);
    scanner->setActiveScan(true);
    scanner->start(SCAN_DURATION, false);
}

void ClientHandler::loop() {}

void ClientCallback::onConnect(BLEClient *client) { /* Nothing to do */ }

void ClientCallback::onDisconnect(BLEClient *client) {
    connected = false;
}

//Triggered for each advertised ble device found during an active scan. bascially what do to when
// you encounter a new device
void AdvertisedDeviceCallback::onResult(BLEAdvertisedDevice advertisedDevice) {
    if (advertisedDevice.haveServiceUUID() && advertisedDevice.isAdvertisingService(serviceUUID)) {
        BLEDevice::getScan()->stop();
        server = new BLEAdvertisedDevice(advertisedDevice);
        attemptConnect = true;
        attemptScan = true;
    }
}

bool ClientHandler::connectToServer() {
    // This creates a client object and returns a pointer
    BLEClient *client = BLEDevice::createClient();

    //This sets the callbacks for the client. These are called from device if there are
    // connection or disconnection events
    client->setClientCallbacks(new ClientCallback());

    //Attempts to establish a connection to server. If the conneciton fails connect will return
    // true the program will enter the loop and return false
    if (!client->connect(server)) {
        return false;
    }

    //probably dont need this
    client->setMTU(517);

    //todo perhaps do this in a more dynamic fashion
    //This tries to get the service specified by the service UUID from the connected server. If
    // it is null, that service is not available so the client disconnects and the program
    // returns false
    BLERemoteService *remoteService = client->getService(serviceUUID);
    if (remoteService == nullptr) {
        client->disconnect();
        return false;
    }

    // See above but for IMU characteristc
    IMUCharacteristic = remoteService->getCharacteristic(imuCharacteristicUUID);
    if (IMUCharacteristic == nullptr) {
        client->disconnect();
        return false;
    }

    // Checks if reading operations are allowed
    if (IMUCharacteristic->canRead()) {
        std::string value = IMUCharacteristic->readValue();
        //i dont think i need this
    }

    // Checks if notification operations are allowed
    if (IMUCharacteristic->canNotify()) {
        // If there is a notificaiton it invokes the notifu callback (aka read the quaternion shit)
        IMUCharacteristic->registerForNotify(notifyCallback);
    }

    connected = true;
    return connected;
}

static void notifyCallback(BLERemoteCharacteristic *IMUCharacteristic, uint8_t *data, size_t
length, bool isNotify) {
    if (length == 16) {
        float qw, qx, qy, qz;
        memcpy(&qw, &data[0], sizeof(float));
        memcpy(&qx, &data[4], sizeof(float));
        memcpy(&qy, &data[8], sizeof(float));
        memcpy(&qz, &data[12], sizeof(float));
    }
}

// maybe have the q's be member variables and then have access methods for the rest of the
// program to access the stuff