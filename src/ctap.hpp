#ifndef INCLUDED_ctap_h_
#define INCLUDED_ctap_h_

#include <BLEDevice.h>
#include <BLEDevice.h>
#include <BLEUtils.h>
#include <M5StickC.h>

class ControlPointCallbacks: public BLECharacteristicCallbacks {
    private:
        uint8_t* data = new uint8_t[128];
    public:
        void onWrite(BLECharacteristic *characteristic);
};

#endif