#include "bls.hpp"

void initService() {
  // Create the BLE Device
  BLEDevice::init("FIDO ABS Authenticator");

  // Create the BLE Server
  BLEServer *pServer = BLEDevice::createServer();

  // Create the BLE Service
  BLEService *pService = pServer->createService(SERVICE_UUID);

  // Create the BLE charateristic
  /*
   * CharacteristicName - FIDO Control Point  
   * Property - Write
   * Length - Defined by Vendor(20-512bytes)
   * UUID - f1d0fff1-deaa-ecee-b42f-c9ba7ed623bb
  */
  BLECharacteristic *pCpCharacteristic = pService->createCharacteristic(
    CHARACTERISTIC_CONTROLPOINT_UUID,
    BLECharacteristic::PROPERTY_WRITE
  );
  pCpCharacteristic->setValue("test");

  /*
   * CharacteristicName - FIDO Status
   * Property - Notify
   * Length - N/A
   * UUID - f1d0fff2-deaa-ecee-b42f-c9ba7ed623bb
  */
  BLECharacteristic *pStatusCharacteristic = pService->createCharacteristic(
    CHARACTERISTIC_STATUS_UUID,
    BLECharacteristic::PROPERTY_NOTIFY
  );
  pStatusCharacteristic->setValue("test");

  /*
   * CharacteristicName - FIDO Control Point Length
   * Property - Read
   * Length - 2 bytes
   * UUID - f1d0fff3-deaa-ecee-b42f-c9ba7ed623bb
  */
  BLECharacteristic *pCpLengthCharacteristic = pService->createCharacteristic(
    CHARACTERISTIC_CONTROLPOINTLENGTH_UUID,
    BLECharacteristic::PROPERTY_READ
  );
  pCpLengthCharacteristic->setValue("test");

  /*
   * CharacteristicName - FIDO Sercice Revision Bitfield
   * Property - Read/Write
   * Length - Definede by Vendor(20-512bytes)
   * UUID - f1d0fff4-deaa-ecee-b42f-c9ba7ed623bb
  */
  BLECharacteristic *pSrbBitCharacteristic = pService->createCharacteristic(
    CHARACTERISTIC_SERVICEREVISIONBITFIELD_UUID,
    BLECharacteristic::PROPERTY_READ | 
    BLECharacteristic::PROPERTY_WRITE
  );
  pSrbBitCharacteristic->setValue("0x20");

  /*
   * CharacteristicName - FIDO Service Revision
   * Property - Read
   * Length - Defined by Vendor(20-512bytes)
   * UUID - 00002a28-0000-1000-8000-00805f9b34fb
  */
  BLECharacteristic *pSrbCharacteristic = pService->createCharacteristic(
    CHARACTERISTIC_SERVICEREVISION_UUID,
    BLECharacteristic::PROPERTY_READ
  );
  pSrbCharacteristic->setValue("test");

  // Start the Service
  pService->start();

  // Start advertising
  BLEAdvertising *pAdvertising = BLEDevice::getAdvertising();
  pAdvertising->addServiceUUID(SERVICE_UUID);
  pAdvertising->setScanResponse(true);
  pAdvertising->setMinPreferred(0x0);
  BLEDevice::startAdvertising();
}
