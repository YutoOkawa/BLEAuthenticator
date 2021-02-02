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
    Request request;
    Response response;

    data = characteristic->getData();
    request = parseRequest(data);
    try {
        response = operateCTAPCommand(request);
    } catch (implement_error e) {
        Serial.println(e.what());
    }
    delete[] request.data.commandParameter;
}

// TODO: パース関数のエラー処理
Request ControlPointCallbacks::parseRequest(uint8_t *req) {
    Request request;

    // TODO: commandがなかった場合のエラー処理
    // Commandを取得
    request.command = (unsigned int)*req;
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
        request.data.commandParameter[i] = (uint8_t)*req;
        req++;
    }

    return request;
}

Response ControlPointCallbacks::operateCTAPCommand(Request request) {
    Response response;
    
    switch (request.command) {
        case CommandConstParam::COMMAND_PING:
            response = parsePingCommand(request);

        case CommandConstParam::COMMAND_KEEPALIVE:
            response = parseKeepAliveCommand(request);

        case CommandConstParam::COMMAND_MSG:
            response = parseMsgCommand(request);

        case CommandConstParam::COMMAND_CANCEL:
            response = parseCancelCommand(request);

        case CommandConstParam::COMMAND_ERROR:
            response = parseErrorCommand(request);

        default:
            throw implement_error("Not implement CTAP Command.");
    }
}

Response ControlPointCallbacks::parsePingCommand(Request request) {
    throw implement_error("Not implement PING Command.");
}

Response ControlPointCallbacks::parseKeepAliveCommand(Request request) {
    throw implement_error("Not implement KEEPALIVE Command.");
}

Response ControlPointCallbacks::parseMsgCommand(Request request) {
    AuthenticatorAPI *authAPI;
    Response response;

    if (checkHasParameters(request.data.commandValue)) { /* パラメータを必要とするもの */
        authAPI = new AuthenticatorAPI(request.data.commandValue, request.data.commandParameter, request.llen - request.hlen - 1);
    } else { /* パラメータを必要としなもの */
        authAPI = new AuthenticatorAPI(request.data.commandValue);
    }

    try {
        response = authAPI->operateCommand();
    } catch (implement_error e) {
        throw implement_error(e.what());
    }

    return response;
}

Response ControlPointCallbacks::parseCancelCommand(Request request) {
    throw implement_error("Not implement CANCEL Command.");
}

Response ControlPointCallbacks::parseErrorCommand(Request request) {
    throw implement_error("Not implement ERROR Command.");
}

// エラー:DOMException: GATT Error: Not supported.
// TODO: BLE_Notifyのようにエラーが出ずNotifyできるようにする
// もしかするとEventListenerが問題かも？
void StatusCallbacks::onWrite(BLECharacteristic *characteristic) {
    characteristic->notify();
}