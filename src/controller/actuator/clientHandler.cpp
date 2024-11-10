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
        }

        // Stop scan and set flags
        BLEDevice::getScan()->stop();
        ClientHandler::instance()->setServer(device);
        ClientHandler::instance()->setAttemptConnect(true);
        ClientHandler::instance()->setInitiateScan(true);

        Log.infoln("AdvertisedDeviceCallbacks::onResult - Found a device with the correct service"
                   " UUID. Set connect flags to true");
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
        return false;
    }

    // Attempt to allocate memory for the singleton inst
    inst = new(std::nothrow)
    ClientHandler(SERVICE_UUID, IMU_CHARACTERISTIC_UUID, DEVICE_NAME);
    if (!inst) {
        Log.errorln("ClientHandler::initialize - Memory allocation failed for ClientHandler "
                    "instance");
        return false;
    }

    // Attempt to retrieve a Scan
    auto *scanner = BLEDevice::getScan();
    if (!scanner) {
        Log.errorln("ClientHandler::initialize - Failed to retrieve scanner");
        return false;
    }

    // Attempt to retrieve an AdvertisedDeviceCallback
    auto callback = std::make_shared<AdvertisedDeviceCallbacks>();
    if (!callback) {
        delete scanner;
        Log.errorln("ClientHandler::initialize - Memory allocation failed for "
                    "AdvertisedDeviceCallbacks");
        return false;
    }

    // Configure the scan
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
    if (inst == nullptr) {
        throw std::runtime_error("ClientHandler::initialize must be called first");
    }

    return inst;
}

void ClientHandler::loop() {
    if (attemptConnect) {
        if (connectToServer()) {
            //we are connected
        } else {
            //we are not connected
        }

        attemptConnect = false;
    }

    if (connected) {
        // do stuff once we are connected
    } else if (initiateScan) {
        BLEDevice::getScan()->start(0);
    }
}

void ClientHandler::setConnected(const bool &newConnected) {
    connected = newConnected;
}

void ClientHandler::setServer(BLEAdvertisedDevice *newServer) {
    delete server;
    server = newServer;
}

void ClientHandler::setAttemptConnect(const bool &newAttemptConnect) {
    attemptConnect = newAttemptConnect;
}

void ClientHandler::setInitiateScan(const bool &newInitiateScan) {
    initiateScan = newInitiateScan;
}

BLEUUID ClientHandler::getServiceUUID() const {
    return serviceUUID;
}

ClientHandler::ClientHandler(const std::string &SERVICE_UUID,
                             const std::string &IMU_CHARACTERISTIC_UUID,
                             const std::string &DEVICE_NAME)
        : serviceUUID(SERVICE_UUID), imuCharacteristicUUID(IMU_CHARACTERISTIC_UUID),
          attemptConnect(false), connected(false), initiateScan(false), server
                  (nullptr), IMUCharacteristic(nullptr) {
    // Create the BLE device
    BLEDevice::init(DEVICE_NAME);
}

bool ClientHandler::connectToServer() {
    // This creates a client object and returns a pointer
    BLEClient *client = BLEDevice::createClient();

    //This sets the callbacks for the client. These are called from device if there are
    // connection or disconnection events
    client->setClientCallbacks(new ClientCallbacks());

    //Attempts to establish a connection to server. If the conneciton fails connect will return
    // false negated to true and the program will enter the loop and return false
    if (!client->connect(server)) {
        return false;
    }

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
        // If there is a notification that says hey there is new data, it invokes the notify
        // callback (aka read the quaternion shit)
        IMUCharacteristic->registerForNotify(notifyCallback);
    }

    connected = true;
    return connected;
}

void ClientHandler::notifyCallback(BLERemoteCharacteristic *IMUCharacteristic, uint8_t
*data, size_t length, bool isNotify) {
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