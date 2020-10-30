#include <M5StickC.h>
#include "bls.hpp"

void setup() {
  // put your setup code here, to run once:
  M5.begin();

  M5.Lcd.println("FIDO Authenticator");
  initService();
  M5.Lcd.println("BLE START!");
}

void loop() {
  // put your main code here, to run repeatedly:
}