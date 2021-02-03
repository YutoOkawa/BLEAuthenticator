#include "ble.hpp"

/* ----------------------ServiceConstParam---------------------- */
const char ServiceConstParam::SERVICE_UUID[37] = "0000fffd-0000-1000-8000-00805f9b34fa";

/* ----------------------CharacteristicConstParam---------------------- */
const char CharacteristicConstParam::CHARACTERISTIC_CONTROLPOINT_UUID[37] = "f1d0fff1-deaa-ecee-b42f-c9ba7ed623bb";
const char CharacteristicConstParam::CHARACTERISTIC_STATUS_UUID[37] = "f1d0fff2-deaa-ecee-b42f-c9ba7ed623bb";
const char CharacteristicConstParam::CHARACTERISTIC_CONTROLPOINTLENGTH_UUID[37] = "f1d0fff3-deaa-ecee-b42f-c9ba7ed623bb";
const char CharacteristicConstParam::CHARACTERISTIC_SERVICEREVISIONBITFIELD_UUID[37] = "f1d0fff4-deaa-ecee-b42f-c9ba7ed623bb";
const char CharacteristicConstParam::CHARACTERISTIC_SERVICEREVISION_UUID[37] = "00002a28-0000-1000-8000-00805f9b34fb";

void CTAPBLE::init() {
    // Create the BLE Device
    BLEDevice::init("FIDO ABS Authenticator");
}

void CTAPBLE::startService() {
    // Create the BLE Server
    pServer = BLEDevice::createServer();

    // Create the BLE Service
    pService = pServer->createService(ServiceConstParam::SERVICE_UUID);

    // Create the BLE Characteristic
    pCpCharacteristic = pService->createCharacteristic(
        CharacteristicConstParam::CHARACTERISTIC_CONTROLPOINT_UUID,
        BLECharacteristic::PROPERTY_WRITE
    );
    pCpCharacteristic->setCallbacks(new ControlPointCallbacks());

    pStatusCharacteristic = pService->createCharacteristic(
        CharacteristicConstParam::CHARACTERISTIC_STATUS_UUID,
        BLECharacteristic::PROPERTY_NOTIFY
    );
    pStatusCharacteristic->setCallbacks(new StatusCallbacks());

    pCpLengthCharacteristic = pService->createCharacteristic(
        CharacteristicConstParam::CHARACTERISTIC_CONTROLPOINTLENGTH_UUID,
        BLECharacteristic::PROPERTY_READ
    );
    pCpLengthCharacteristic->setValue("0x9b");

    pSrbBitCharacteristic = pService->createCharacteristic(
        CharacteristicConstParam::CHARACTERISTIC_SERVICEREVISIONBITFIELD_UUID,
        BLECharacteristic::PROPERTY_READ | 
        BLECharacteristic::PROPERTY_WRITE
    );
    pSrbBitCharacteristic->setValue("0x20");

    pSrbCharacteristic = pService->createCharacteristic(
        CharacteristicConstParam::CHARACTERISTIC_SERVICEREVISION_UUID,
        BLECharacteristic::PROPERTY_READ
    );
    pSrbCharacteristic->setValue("NOT recommended.");

    // Start the Service
    pService->start();

}

void CTAPBLE::startAdvertise() {
    // Start Adveritising
    pAdvertising = BLEDevice::getAdvertising();
    pAdvertising->addServiceUUID(ServiceConstParam::SERVICE_UUID);
    pAdvertising->setScanResponse(true);
    pAdvertising->setMinPreferred(0x0);
    BLEDevice::startAdvertising();
}

void CTAPBLE::setServer(BLEServer *pServer) {
    this->pServer = pServer;
}

void CTAPBLE::setService(BLEService *pService) {
    this->pService = pService;
}

void CTAPBLE::setCpCharacteristic(BLECharacteristic *pCpCharacteristic) {
    this->pCpCharacteristic = pCpCharacteristic;
}

void CTAPBLE::setStatusCharacteristic(BLECharacteristic *pStatusCharacteristic) {
    this->pStatusCharacteristic = pStatusCharacteristic;
}

void CTAPBLE::setCpLengthCharacteristic(BLECharacteristic *pCpLengthCharacteristic) {
    this->pCpLengthCharacteristic = pCpLengthCharacteristic;
}

void CTAPBLE::setSrbBitCharacteristic(BLECharacteristic *pSrbBitCharacteristic) {
    this->pSrbBitCharacteristic = pSrbBitCharacteristic;
}

void CTAPBLE::setSrbCharacteristic(BLECharacteristic *pSrbCharacteristic) {
    this->pSrbCharacteristic = pSrbCharacteristic;
}

void CTAPBLE::setAdvertising(BLEAdvertising *pAdvertising) {
    this->pAdvertising = pAdvertising;
}

BLEServer *CTAPBLE::getServer() {
    return this->pServer;
}

BLEService *CTAPBLE::getService() {
    return this->pService;
}

BLECharacteristic *CTAPBLE::getCpCharacteristic() {
    return this->pCpCharacteristic;
}

BLECharacteristic *CTAPBLE::getStatusCharacteristic() {
    return this->pStatusCharacteristic;
}

BLECharacteristic *CTAPBLE::getCpLengthCharacteristic() {
    return this->pCpLengthCharacteristic;
}

BLECharacteristic *CTAPBLE::getSrbBitCharacteritsic() {
    return this->pSrbBitCharacteristic;
}

BLECharacteristic *CTAPBLE::getSrbCharacteritsic() {
    return this->pSrbCharacteristic;
}

BLEAdvertising *CTAPBLE::getAdvertising() {
    return this->pAdvertising;
}
