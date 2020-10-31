#include "ctap.hpp"

void ControlPointCallbacks::onWrite(BLECharacteristic *characteristic) {
    data = characteristic->getData();
    M5.Lcd.println((const char*)data);
}