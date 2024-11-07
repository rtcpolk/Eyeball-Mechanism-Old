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

bool ClientHandler::connectToServer() {
    // This creates a client object and returns a pointer
    BLEClient *client = BLEDevice::createClient();

    //This sets the callbacks for the client. These are called from device if there are
    // connection or disconnection events
    client->setClientCallbacks(new ClientCallback());

    //Attempts to establish a connection to server. If the conneciton fails connect will return
    // false negated to true and the program will enter the loop and return false
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
        // If there is a notificaiton that says hey there is new data, it invokes the notifu
        // callback (aka read the quaternion shit)
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



// maybe have the q's be member variables and then have access methods for the rest of the
// program to access the stuff

/*Step-by-Step Flow:
Connecting to the server:

Your client connects to the BLE server and checks if the desired characteristic (IMU) supports notifications.
Calling registerForNotify:

The registerForNotify method registers your client to receive notifications for the IMU characteristic.
If notifications are enabled, it writes the correct value to the 0x2902 descriptor to tell the server to send notifications when the data changes.
Receiving a Notification:

When the IMU characteristic changes on the server (e.g., new sensor data), the server sends a notification to your client.
The BLE stack on your ESP32 receives this notification and triggers the notifyCallback method you provided.
Callback Execution:

Inside notifyCallback, you extract the quaternion values (represented as floats) from the data array and process them accordingly.
This is where the actual data handling happens: you can then use the quaternion values (qw, qx, qy, qz) for whatever processing you need, such as sensor fusion or displaying the IMU data.*/

//todo add logging and excpetion saftey