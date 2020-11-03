#include "ctap.hpp"

const int CommandConstParam::COMMAND_PING = 0x81;
const int CommandConstParam::COMMAND_KEEPALIVE = 0x82;
const int CommandConstParam::COMMAND_MSG = 0x83;
const int CommandConstParam::COMMAND_CANCEL = 0xbe;
const int CommandConstParam::COMMAND_ERROR = 0xbf;

const int KeepAliveConstParam::KEEPALIVE_PROCESSING = 0x01;
const int KeepAliveConstParam::KEEPALIVE_UP_NEEDED = 0x02;

const int ErrorConstParam::ERR_INVALID_CMD = 0x01;
const int ErrorConstParam::ERR_INVALID_PAR = 0x02;
const int ErrorConstParam::ERR_INVALID_LEN = 0x03;
const int ErrorConstParam::ERR_INVALID_SEQ = 0x04;
const int ErrorConstParam::ERR_REQ_TIMEOUT = 0x05;
const int ErrorConstParam::ERR_BUSY = 0x06;
const int ErrorConstParam::ERR_OTHER = 0x7f;

void ControlPointCallbacks::onWrite(BLECharacteristic *characteristic) {
    data = characteristic->getData();
    M5.Lcd.println((const char*)data);
}