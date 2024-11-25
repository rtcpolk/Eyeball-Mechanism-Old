// Author: Robert Polk
// Copyright (c) 2024 BLINK. All rights reserved.
// Last Modified: 11/24/2024

#include "controller/clientHandler.h"

void ClientCallbacks::onConnect(NimBLEClient *connectedClient) {
    Log.infoln("Connected to the server");
    /** After connection we should change the parameters if we don't need fast response times.
        *  These settings are 150ms interval, 0 latency, 450ms timout.
        *  Timeout should be a multiple of the interval, minimum is 100ms.
        *  I find a multiple of 3-5 * the interval works best for quick response/reconnect.
        *  Min interval: 120 * 1.25ms = 150, Max interval: 120 * 1.25ms = 150, 0 latency, 60 * 10ms = 600ms timeout
        */
    connectedClient->updateConnParams(120, 120, 0, 60); //todo figure this out
}

void ClientCallbacks::onDisconnect(NimBLEClient *disconnectedClient, int reason) {
    Log.warningln("Disconnected from the server (code %d). Starting scan", reason);
    NimBLEDevice::getScan()->start(ClientHandler::SCAN_TIME);
}

void ScanCallbacks::onResult(NimBLEAdvertisedDevice *advertisedDevice) {
    Log.trace("Advertised Device found: ");
    Log.traceln(advertisedDevice->toString().c_str());

    if (advertisedDevice->isAdvertisingService(NimBLEUUID(ClientHandler::SERVICE_UUID))) {
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

void
notifyCallback(NimBLERemoteCharacteristic *pRemoteCharacteristic, uint8_t *pData, size_t length,
               bool isNotify) {
    Log.infoln("Notify");
}

// Set static variables
NimBLEAdvertisedDevice *ClientHandler::advDevice = nullptr;
std::string ClientHandler::SERVICE_UUID = "";
uint8_t ClientHandler::SCAN_TIME = 0;
bool ClientHandler::doConnect = false;
ClientHandler *ClientHandler::inst = nullptr;
ClientCallbacks ClientHandler::clientCallback;
ScanCallbacks ClientHandler::scanCallback;
bool ClientHandler::initialized = false;

ClientHandler::~ClientHandler() { inst = nullptr; }

void ClientHandler::initialize(const std::string &SERVICE_UUID_TO_SET, const std::string
&IMU_CHARACTERISTIC_UUID_TO_SET, const std::string &DEVICE_NAME, const uint8_t &SCAN_TIME_TO_SET,
                               const uint32_t &SCAN_WINDOW, const uint32_t &SCAN_INTERVAL) {
    Log.traceln("ClientHandler::initialize - Begin");

    // Set UUIDs
    SERVICE_UUID = SERVICE_UUID_TO_SET;
    IMU_CHARACTERISTIC_UUID = IMU_CHARACTERISTIC_UUID_TO_SET;
    // Check and set scan time
    SCAN_TIME = SCAN_TIME_TO_SET * 1000; // In ms

    // Initialize the BLE Device
    NimBLEDevice::init(DEVICE_NAME);

    // Configure and start scan
    NimBLEScan *scanner = NimBLEDevice::getScan();
    scanner->setScanCallbacks(&scanCallback);
    scanner->setInterval(SCAN_INTERVAL);
    scanner->setWindow(SCAN_WINDOW);
    scanner->setActiveScan(true);
    scanner->start(SCAN_TIME);
    Log.traceln("ClientHandler::initialize - Begin");
}

ClientHandler *ClientHandler::instance() {
    if (inst == nullptr) {
        inst = new ClientHandler();
    }

    return inst;
}

void ClientHandler::loop() {
    while (true) {

        if (doConnect) {
            Log.traceln("doconnect true");
            if (connectToServer()) {
                Log.infoln("Successfully connected to the server");
                doConnect = false;
            } else {
                Log.traceln("Failed to connect to the server");
            }
        }
        delay(10);
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
    Log.info("RSSI: ");
    Log.infoln("%d", client->getRssi());

    // Check the characteristics for the correct properties
    Log.traceln("ClientHandler::connectToServer - Checking the characteristics");
    remoteService = client->getService(SERVICE_UUID);
    if (remoteService) {
        remoteIMUCharacteristic = remoteService->getCharacteristic(IMU_CHARACTERISTIC_UUID);

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