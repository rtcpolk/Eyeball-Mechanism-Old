// Author: Robert Polk
// Copyright (c) 2024 BLINK. All rights reserved.
// Last Modified: 11/25/2024

#include "mechanism/clientHandler.h"

void ClientCallbacks::onConnect(NimBLEClient *connectedClient) {
    Log.infoln("Connected to the server");
    connectedClient->updateConnParams(120, 120, 0, 60); //todo figure this out
}

void ClientCallbacks::onDisconnect(NimBLEClient *disconnectedClient, int reason) {
    Log.warningln("Disconnected from the server (code %d). Starting scan", reason);
    NimBLEDevice::getScan()->start(ClientHandler::scanTime);
}

void ScanCallbacks::onResult(NimBLEAdvertisedDevice *advertisedDevice) {
    Log.trace("Advertised Device found: ");
    Log.traceln(advertisedDevice->toString().c_str());

    // Check if the device has the correct service UUID
    if (advertisedDevice->isAdvertisingService(NimBLEUUID(ClientHandler::serviceUUID))) {
        Log.traceln("Found a server with the correct service");
        NimBLEDevice::getScan()->stop();
        ClientHandler::advDevice = advertisedDevice;
        ClientHandler::doConnect = true;
    }

    Log.traceln("onResult end");
}

void ScanCallbacks::onScanEnd(NimBLEScanResults results) {
    Log.traceln("ScanCallbacks::onScanEnd - Scan ended");
}

// Set static variables
NimBLEAdvertisedDevice *ClientHandler::advDevice = nullptr;
std::string ClientHandler::serviceUUID = "";
uint32_t ClientHandler::scanTime = 5 * 1000;
bool ClientHandler::doConnect = false;
ClientHandler *ClientHandler::inst = nullptr;
ClientCallbacks ClientHandler::clientCallback;
ScanCallbacks ClientHandler::scanCallback;
bool ClientHandler::initialized = false;
std::string ClientHandler::IMUCharacteristicUUID = "";
Quaternion ClientHandler::quaternion;

ClientHandler::~ClientHandler() { inst = nullptr; }

ClientHandler *ClientHandler::instance() {
    if (inst == nullptr) {
        inst = new ClientHandler();
    }

    return inst;
}

void ClientHandler::initialize(const std::string &SERVICE_UUID, const std::string
&IMU_CHARACTERISTIC_UUID, const std::string &DEVICE_NAME, const uint8_t &SCAN_TIME,
                               const uint32_t &SCAN_WINDOW, const uint32_t &SCAN_INTERVAL) {
    Log.traceln("ClientHandler::initialize - Begin");
//todo fix static initialize
    // Set UUIDs
    serviceUUID = SERVICE_UUID;
    IMUCharacteristicUUID = IMU_CHARACTERISTIC_UUID;
    // Check and set scan time
    scanTime = SCAN_TIME;

    // Initialize the BLE Device
    NimBLEDevice::init(DEVICE_NAME);

    // Configure and start scan
    NimBLEScan *scanner = NimBLEDevice::getScan();
    scanner->setScanCallbacks(&scanCallback);
    scanner->setInterval(SCAN_INTERVAL);
    scanner->setWindow(SCAN_WINDOW);
    scanner->setActiveScan(true);
    scanner->start(SCAN_TIME);
    Log.traceln("ClientHandler::initialize - End");
}

void
ClientHandler::notifyCallback(NimBLERemoteCharacteristic *remoteCharacteristic, uint8_t *pData,
                              size_t length,
                              bool isNotify) {
    //todo add a buffer?
    if (remoteCharacteristic->getUUID() == BLEUUID(IMUCharacteristicUUID) && isNotify) {
        if (length == 16) {
            memcpy(&quaternion.w, &pData[0], sizeof(float));
            memcpy(&quaternion.x, &pData[4], sizeof(float));
            memcpy(&quaternion.y, &pData[8], sizeof(float));
            memcpy(&quaternion.z, &pData[12], sizeof(float));

            Log.verboseln("\tQuat:\t%D\t%D\t%D\t%D", quaternion.w, quaternion.x, quaternion.y,
                          quaternion.z);

        } else {
            Log.warningln("ClientHandler::notifyCallback - Unexpected data length received");
        }
    } else {
        Log.warningln("ClientHandler::notifyCallback - Unexpected characteristic or trigger");
    }
}

const Quaternion &ClientHandler::getQuaternion() const {
    return quaternion;
}

void ClientHandler::loop() {
    while (true) {
        try {

            if (doConnect) {
                if (connectToServer()) {
                    Log.traceln("Successfully connected to the server");
                    doConnect = false;
                } else {
                    Log.traceln("Failed to connect to the server");
                }
            }
            delay(10);
        } catch (const std::exception &ex) {
            Log.errorln("ClientHandler::Loop execution failed - %s", ex.what());
        } catch (...) {
            Log.errorln("ClientHandler::Loop execution failed - Unknown Error");
        }
    }
}

bool ClientHandler::connectToServer() {
    Log.traceln("ClientHandler::connectToServer - Begin");

    // Ptrs for the method
    NimBLEClient *client = nullptr;
    NimBLERemoteService *remoteService = nullptr;
    NimBLERemoteCharacteristic *remoteIMUCharacteristic = nullptr;

    // Check if there is a client to reuse
    Log.traceln("ClientHandler::connectToServer - Checking for client reuse");
    if (NimBLEDevice::getCreatedClientCount()) {
        client = NimBLEDevice::getClientByPeerAddress(advDevice->getAddress());

        if (client) {   // Already know the device
            if (!client->connect(advDevice, false)) {
                Log.errorln("ClientHandler::connectToServer - Reconnect failed");
                return false;
            }
            Log.traceln("ClientHandler::connectToServer - Reconnect success");
        } else {    // Don't know the device
            client = NimBLEDevice::getDisconnectedClient();
        }
    }

    // If there is no client to reuse, create a new one
    Log.traceln("ClientHandler::connectToServer - Creating a client if there is no reuse");
    if (!client) {
        if (NimBLEDevice::getCreatedClientCount() >= NIMBLE_MAX_CONNECTIONS) {
            Log.errorln("ClientHandler::connectToServer - Max clients reached. No connections "
                        "available");
            return false;
        }

        // Create the client and set callbacks
        client = NimBLEDevice::createClient();
        client->setClientCallbacks(&clientCallback, false);
        Log.traceln("New Client created");

        // Set connection params
        client->setConnectionParams(12, 12, 0, 51); //todo figure this out
        client->setConnectTimeout(5 * 1000);

        // See if the created client connected
        if (!client->connect(advDevice)) {
            NimBLEDevice::deleteClient(client);
            Log.errorln("ClientHandler::connectToServer - Failed to connect. Deleted client");
            return false;
        }
    }

    // Ensure client is connected
    Log.traceln("ClientHandler::connectToServer - Ensuring client is connected");
    if (!client->isConnected()) {
        if (!client->connect(advDevice)) {
            Log.errorln("ClientHandler::connectToServer - Failed to connect");
            return false;
        }
    }
    Log.info("Connected to: ");
    Log.infoln(client->getPeerAddress().toString().c_str());
    Log.trace("RSSI: ");
    Log.traceln("%d", client->getRssi());

    // Check the characteristics for the correct properties
    Log.traceln("ClientHandler::connectToServer - Checking the characteristics");
    remoteService = client->getService(serviceUUID);
    if (remoteService) {
        remoteIMUCharacteristic = remoteService->getCharacteristic(IMUCharacteristicUUID);

        if (remoteIMUCharacteristic) {
            // Make sure read is supported
            if (!remoteIMUCharacteristic->canRead()) {
                Log.errorln("ClientHandler::connectToServer - IMU Characteristic does not support "
                            "read");
                client->disconnect();
                return false;
            }

            // Make sure notify is supported and subscribe
            if (remoteIMUCharacteristic->canNotify()) {
                if (!remoteIMUCharacteristic->subscribe(true, notifyCallback)) {
                    Log.errorln("ClientHandler::connectToServer - Failed to subscribe to IMU "
                                "Characteristic");
                    client->disconnect();
                    return false;
                }
            }
        }
    } else {
        Log.errorln("ClientHandler::connectToServer - Service not found");
        return false;
    }

    Log.traceln("ClientHandler::connectToServer - End");
    return true;
}