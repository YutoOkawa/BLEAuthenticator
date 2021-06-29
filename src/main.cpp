#include <M5StickC.h>
#include "ble.hpp"

CTAPBLE Device;
int maxResponseSize = 255;

void notify();

void setup() {
    // put your setup code here, to run once:
    M5.begin();
    Serial.begin(115200);
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
    delay(1);
}

/* 接続されれば起動 */
/* ControlPointで値の処理が終了すればフラグが立ち上がり、Statusに値をセットしてNotifyを行う */
void notify() {
    if (Device.getControlPoint()->getFlag()) {
        /* 送信データとサイズを取得 */
        unsigned int length = Device.getControlPoint()->getResponseDataLength();
        uint8_t *data = Device.getControlPoint()->getResponseData();
        if (length > maxResponseSize) { /* 最大サイズより大きい場合は分割して送信 */
            int count = (length/maxResponseSize); /* 最大サイズで送信する回数 */
            int lastSize = length - (count * maxResponseSize); /* あまりサイズ */
            for (int i=0; i<count+1; i++) {
                int dataSize = 0;
                if (i<count) {
                    dataSize = maxResponseSize;
                } else {
                    dataSize = lastSize;
                }
                uint8_t *dividedData = new uint8_t[dataSize];
                memcpy(dividedData, data, dataSize); /* 送信パケット作成 */
                data += dataSize;
                Device.getStatusCharacteristic()->setValue(dividedData, dataSize);
                Device.getStatusCharacteristic()->notify();
                delete dividedData;
            }
        } else { /* 最大サイズより小さい場合はそのまま送信 */ 
            /* 単体送信の場合エラーが起きていそう */
            Device.getStatusCharacteristic()->setValue(data, length);
            Device.getStatusCharacteristic()->notify();
        }
        Device.getControlPoint()->setFlag(false);
    }
}