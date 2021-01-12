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

const int ParseErrorConstParam::PARSE_ERR_CMD = -1;
const int ParseErrorConstParam::PARSE_ERR_HLEN = -2;
const int ParseErrorConstParam::PARSE_ERR_LLEN = -3;

void ControlPointCallbacks::onWrite(BLECharacteristic *characteristic) {
    Request request;
    data = characteristic->getData();
    request = parseRequest(data);

    if (request.error == 0) {
        // TODO: リクエストに応じた処理の記述
        M5.Lcd.printf("hlen:%d llen:%dCMD:%x\n", request.hlen, request.llen, request.data.commandValue);
        for (int i=request.hlen; i<request.llen-1; i++) {
            M5.Lcd.println(request.data.commandParameter[i], HEX);
        }
        delete[] request.data.commandParameter;
    } else {
        M5.Lcd.printf("Parse Error: %d", request.error);
    }
}

// TODO: パース関数のエラー処理
Request ControlPointCallbacks::parseRequest(uint8_t *req) {
    Request request;
    request.error = 0;

    // TODO: commandがなかった場合のエラー処理
    // Commandを取得
    request.command = (unsigned int)*req;
    switch(request.command) {
        case CommandConstParam::COMMAND_PING:
            M5.Lcd.println("PING"); break;
        case CommandConstParam::COMMAND_KEEPALIVE:
            M5.Lcd.println("KEEP"); break;
        case CommandConstParam::COMMAND_MSG:
            M5.Lcd.println("MSG"); break;
        case CommandConstParam::COMMAND_CANCEL:
            M5.Lcd.println("CANCEL"); break;
        default:
            M5.Lcd.println("ERROR"); break;
    }
    req++;

    // TODO: HLENがなかった場合のエラー処理
    // HLENを取得
    request.hlen = (unsigned int)*req;
    req++;

    // TODO: LLENがなかった場合のエラー処理
    // LLENを取得
    request.llen = (unsigned int)*req;
    req++;

    // TODO: Dataがなかった場合のエラー処理
    // Dataを取得
    // Command Valueを取得
    request.data.commandValue = (unsigned int)*req;
    req++;
    // Command Parameterを取得
    request.data.commandParameter = new uint8_t[request.llen-1];
    for (int i=request.hlen; i<request.llen-1; i++) {
        request.data.commandParameter[i] = *req;
        req++;
    }

    return request;
}

// エラー:DOMException: GATT Error: Not supported.
// TODO: BLE_Notifyのようにエラーが出ずNotifyできるようにする
// もしかするとEventListenerが問題かも？
void StatusCallbacks::onWrite(BLECharacteristic *characteristic) {
    characteristic->notify();
}