#include <M5StickC.h>
#include "ble.hpp"

CTAPBLE Device;

void notify();

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
    if (Device.getConnectServer()->getConnect()) {
        notify();
    } else {
        // Serial.println("Device is not connected.");
    }
    delay(2000);
}

/* 接続されれば起動 */
/* ControlPointで値の処理が終了すればフラグが立ち上がり、Statusに値をセットしてNotifyを行う */
void notify() {
    if (Device.getControlPoint()->getFlag()) {
        Device.getStatusCharacteristic()->setValue(Device.getControlPoint()->getResponseData(), Device.getControlPoint()->getResponseDataLength());
        Device.getStatusCharacteristic()->notify();
        Device.getControlPoint()->setFlag(false);
    }
}