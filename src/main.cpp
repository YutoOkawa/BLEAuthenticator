#include <M5StickC.h>
#include "bls.hpp"

void setup() {
    // put your setup code here, to run once:
    CTAPBLE Device;
    M5.begin();
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