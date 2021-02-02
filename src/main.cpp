#include <M5StickC.h>
#include "ble.hpp"

CTAPBLE Device;

void setup() {
    // put your setup code here, to run once:
    M5.begin();
    Serial.begin(9600);
    Device.init();
    Device.startService();
    M5.Lcd.println("FIDO Authenticator");
    Device.startAdvertise();
    M5.Lcd.println("BLE START!");
}

void loop() {
    // put your main code here, to run repeatedly:
    delay(200);
}