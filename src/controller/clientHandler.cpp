// Author: Robert Polk
// Copyright (c) 2024 BLINK. All rights reserved.
// Last Modified: 11/07/24

#include "controller/actuator/clientHandler.h"

void ClientCallbacks::onConnect(BLEClient *client) {
    Log.traceln("ClientCallbacks::onConnect - Start");
    Log.infoln("ClientCallbacks::onConnect - Connection event occurred");
    Log.traceln("ClientCallbacks::onConnect - Stop");
}

void ClientCallbacks::onDisconnect(BLEClient *client) {
    Log.traceln("ClientCallbacks::onDisconnect - Start");
    Log.warningln("ClientCallbacks::onDisconnect - Disconnection event occurred");

    ClientHandler::instance()->setConnected(false);

    Log.traceln("ClientCallbacks::onDisconnect - Stop");
}

void AdvertisedDeviceCallbacks::onResult(BLEAdvertisedDevice advertisedDevice) {
    Log.traceln("AdvertisedDeviceCallbacks::onResult - Start");
    Log.trace("BLE Advertised device found: ");
    Log.traceln(advertisedDevice.toString().c_str());

    // Check if the device contains the correct service UUIDs
    if (advertisedDevice.haveServiceUUID() && advertisedDevice.isAdvertisingService
            (ClientHandler::instance()->getServiceUUID())) {
        // Attempt to make a BLEAdvertisedDevice (the server)
        auto *device = new(std::nothrow)
                BLEAdvertisedDevice(advertisedDevice);
        if (!device) {
            Log.errorln("AdvertisedDeviceCallbacks::onResult - Memory allocation failed for "
                        "BLEAdvertisedDevice");
            Log.traceln("AdvertisedDeviceCallbacks::onResult - Stop");
            return;
        }

        // Stop scan and set flags
        BLEDevice::getScan()->stop();
        ClientHandler::instance()->setServer(device);
        ClientHandler::instance()->setAttemptConnect(true);
        ClientHandler::instance()->setInitiateScan(true);

        Log.infoln("AdvertisedDeviceCallbacks::onResult - Found a device with the correct service"
                   " UUID");
    } else {
        Log.traceln("AdvertisedDeviceCallbacks::onResult - Advertised device does not contain "
                    "the "
                    "correct service UUID");
    }

    Log.traceln("AdvertisedDeviceCallbacks::onResult - Stop");
}

// Set static inst to null
ClientHandler *ClientHandler::inst = nullptr;

ClientHandler::~ClientHandler() {
    Log.traceln("ClientHandler::~ClientHandler - Start");

    inst = nullptr;

    Log.traceln("ClientHandler::~ClientHandler - Stop");
}

bool ClientHandler::initialize(const std::string &SERVICE_UUID,
                               const std::string &IMU_CHARACTERISTIC_UUID,
                               const std::string &DEVICE_NAME) {
    Log.traceln("ClientHandler::initialize - Start");

    // Only initialize once
    if (inst != nullptr) {
        Log.errorln("ClientHandler::initialize - Already initialized");
        Log.traceln("ClientHandler::initialize - Stop");
        return false;
    }

    // Attempt to allocate memory for the singleton inst
    inst = new(std::nothrow)
            ClientHandler(SERVICE_UUID, IMU_CHARACTERISTIC_UUID, DEVICE_NAME);
    if (!inst) {
        Log.errorln("ClientHandler::initialize - Memory allocation failed for ClientHandler "
                    "instance");
        Log.traceln("ClientHandler::initialize - Stop");
        return false;
    }

    // Attempt to retrieve a Scan
    auto *scanner = BLEDevice::getScan();
    if (!scanner) {
        delete inst;

        Log.errorln("ClientHandler::initialize - Failed to retrieve scanner");
        Log.traceln("ClientHandler::initialize - Stop");
        return false;
    }

    // Attempt to retrieve an AdvertisedDeviceCallback
    auto callback = std::make_shared<AdvertisedDeviceCallbacks>();
    if (!callback) {
        delete inst;
        delete scanner;

        Log.errorln("ClientHandler::initialize - Memory allocation failed for "
                    "AdvertisedDeviceCallbacks");
        Log.traceln("ClientHandler::initialize - Stop");
        return false;
    }

    // Configure the scanner
    scanner->setAdvertisedDeviceCallbacks(callback.get());
    scanner->setInterval(SCAN_INTERVAL);
    scanner->setWindow(SCAN_WINDOW);
    scanner->setActiveScan(true);

    // Scan for devices. This ends up calling onResult
    Log.infoln("ClientHandler::initialize - Starting BLE scan");
    scanner->start(SCAN_DURATION, false);

    Log.infoln("ClientHandler::initialize - ClientHandler initialized successfully");
    Log.traceln("ClientHandler::initialize - Stop");
    return true;
}

ClientHandler *ClientHandler::instance() {
    Log.traceln("ClientHandler::instance - Start");

    if (inst == nullptr) {
        Log.errorln("ClientHandler::instance - Initialize must be called first");
    }

    Log.traceln("ClientHandler::instance - Stop");
    return inst;
}

void ClientHandler::loop() {
    Log.traceln("ClientHandler::loop - Start");
}

void ClientHandler::setConnected(const bool &newConnected) {
    Log.traceln("ClientHandler::setConnected - Start");

    connected = newConnected;

    Log.traceln("ClientHandler::setConnected - Stop");
}

void ClientHandler::setServer(BLEAdvertisedDevice *newServer) {
    Log.traceln("ClientHandler::setServer - Start");

    delete server;
    server = newServer;

    Log.traceln("ClientHandler::setServer - Stop");
}

void ClientHandler::setAttemptConnect(const bool &newAttemptConnect) {
    Log.traceln("ClientHandler::setAttemptConnect - Start");

    attemptConnect = newAttemptConnect;

    Log.traceln("ClientHandler::setAttemptConnect - Stop");
}

void ClientHandler::setInitiateScan(const bool &newInitiateScan) {
    Log.traceln("ClientHandler::setInitiateScan - Start");

    initiateScan = newInitiateScan;

    Log.traceln("ClientHandler::setInitiateScan - Stop");
}

BLEUUID ClientHandler::getServiceUUID() const {
    Log.traceln("ClientHandler::getServiceUUID - Start");
    Log.traceln("ClientHandler::getServiceUUID - Stop");
    return serviceUUID;
}

ClientHandler::ClientHandler(const std::string &SERVICE_UUID,
                             const std::string &IMU_CHARACTERISTIC_UUID,
                             const std::string &DEVICE_NAME)
        : serviceUUID(SERVICE_UUID), imuCharacteristicUUID(IMU_CHARACTERISTIC_UUID),
          attemptConnect(false), connected(false), initiateScan(true), server
                  (nullptr), IMUCharacteristic(nullptr) {
    Log.traceln("ClientHandler::ClientHandler - Start");

    // Create the BLE device
    BLEDevice::init(DEVICE_NAME);

    Log.infoln("ClientHandler::ClientHandler - BLE device created");
    Log.traceln("ClientHandler::ClientHandler - Stop");
}

bool ClientHandler::connectToServer() {
    //todo do i need to clean up before returning false?
    Log.traceln("ClientHandler::connectToServer - Start");

    // Attempt to create the client and callback objects, and set the client's callback function
    BLEClient *client = BLEDevice::createClient();
    if (!client) {
        Log.errorln("ClientHandler::connect to server - Memory allocation failed for client "
                    "object");
        Log.traceln("ClientHandler::connectToServer - Stop");
        return false;
    }

    auto *callback = new(std::nothrow) ClientCallbacks();
    if (!callback) {
        Log.errorln("ClientHandler::connect to server - Memory allocation failed for "
                    "ClientCallbacks");
        Log.traceln("ClientHandler::connectToServer - Stop");
        return false;
    }

    client->setClientCallbacks(callback);

    //Attempt to establish a connection to server
    Log.traceln("ClientHandler::connectToServer - Attempting to establish a connection to the "
                "server");
    if (!client->connect(server)) {
        Log.warningln("ClientHandler::connectToServer - Failed to establish a connection");
        Log.traceln("ClientHandler::connectToServer - Stop");
        return false;
    }

    //todo perhaps do this in a more dynamic fashion
    //This tries to get the service specified by the service UUID from the connected server. If
    // it is null, that service is not available so the client disconnects and the program
    // returns false
    Log.traceln("ClientHandler::connectToServer - Attempting to get the service UUID");
    BLERemoteService *remoteService = client->getService(serviceUUID);
    if (remoteService == nullptr) {
        Log.warning("ClientHandler::connectToServer - The service is unavailable");
        client->disconnect();
        Log.traceln("ClientHandler::connectToServer - Stop");
        return false;
    }

    // See above but for IMU characteristic
    Log.traceln("ClientHandler::connectToServer - Attempting to get the IMU characteristic UUID");
    IMUCharacteristic = remoteService->getCharacteristic(imuCharacteristicUUID);
    if (IMUCharacteristic == nullptr) {
        client->disconnect();
        Log.traceln("ClientHandler::connectToServer - Stop");
        return false;
    }

    // Checks if reading operations are allowed
    if (IMUCharacteristic->canRead()) {
        std::string value = IMUCharacteristic->readValue();
        //i dont think i need this
    }

    // Checks if notification operations are allowed
    if (IMUCharacteristic->canNotify()) {
        // If there is a notification that says hey there is new data, it invokes the notify
        // callback (aka read the quaternion shit)
        IMUCharacteristic->registerForNotify(notifyCallback);
    }

    connected = true;

    Log.infoln("ClientHandler::connectToServer - Connected to the server");
    Log.traceln("ClientHandler::connectToServer - Stop");

    return connected;
}

void ClientHandler::notifyCallback(BLERemoteCharacteristic *IMUCharacteristic, uint8_t
*data, size_t length, bool isNotify) {
    Log.traceln("ClientHandler::notifyCallback - Start");

    if (length == 16) {
        float qw, qx, qy, qz;
        memcpy(&qw, &data[0], sizeof(float));
        memcpy(&qx, &data[4], sizeof(float));
        memcpy(&qy, &data[8], sizeof(float));
        memcpy(&qz, &data[12], sizeof(float));

        Serial.print("w:\t");
        Serial.print(qw);
        Serial.print("x:\t");
        Serial.print(qx);
        Serial.print("y:\t");
        Serial.print(qy);
        Serial.print("z:\t");
        Serial.print(qz);
    } else {
        Log.warningln("ClientHandler::notifyCallback - Data is not of length 16");
    }


    Log.traceln("ClientHandler::notifyCallback - Stop");
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

//todo add logging and exception safety

bool ClientHandler::getAttemptConnect() const {
    return attemptConnect;
}

bool ClientHandler::getConnected() const {
    return connected;
}

bool ClientHandler::getInitiateScan() const {
    return initiateScan;
}