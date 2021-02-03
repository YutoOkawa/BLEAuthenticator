#include "ctap.hpp"

/** @brief PINGコマンドの識別子(0x81)  */
const int CommandConstParam::COMMAND_PING = 0x81;

/** @brief KEEPALIVEコマンドの識別子(0x82) */
const int CommandConstParam::COMMAND_KEEPALIVE = 0x82;

/** @brief MSGコマンドの識別子(0x83) */
const int CommandConstParam::COMMAND_MSG = 0x83;

/** @brief CANCELコマンドの識別子(0xbe) */
const int CommandConstParam::COMMAND_CANCEL = 0xbe;

/** @brief ERRORコマンドの識別子(0xbf) */
const int CommandConstParam::COMMAND_ERROR = 0xbf;


/** @brief Status:PROCESSINGの識別子(0x01) */
const int KeepAliveConstParam::KEEPALIVE_PROCESSING = 0x01;

/** @brief Status:UP_NEEDEDの識別子(0x02) */
const int KeepAliveConstParam::KEEPALIVE_UP_NEEDED = 0x02;


/** @brief The command in the request is unknown/invalid (0x01) */
const int ErrorConstParam::ERR_INVALID_CMD = 0x01;

/** @brief the parameters of the command is/are invalid or missing (0x02) */
const int ErrorConstParam::ERR_INVALID_PAR = 0x02;

/** @brief The length of the request is invalid (0x03) */
const int ErrorConstParam::ERR_INVALID_LEN = 0x03;

/** @brief The sequence number is invalid (0x04) */
const int ErrorConstParam::ERR_INVALID_SEQ = 0x04;

/** @brief THe request timed out (0x05) */
const int ErrorConstParam::ERR_REQ_TIMEOUT = 0x05;

/** @brief The deviceis busy and can't accept commands at this time (0x06) */
const int ErrorConstParam::ERR_BUSY = 0x06;

/** @brief Other, unspecified error (0x7f) */
const int ErrorConstParam::ERR_OTHER = 0x7f;

void ControlPointCallbacks::onWrite(BLECharacteristic *characteristic) {
    Request request;
    Response response;

    data = characteristic->getData();
    request = parseRequest(data);
    try {
        response = operateCTAPCommand(request);
    } catch (implement_error e) { /* 未実装コマンドだった場合 */
        Serial.println(e.what());
    }
    delete[] request.data.commandParameter;
}

// TODO: パース関数のエラー処理
/**
 * @brief dataをパースし、Requestを返す
 * 
 * @param req - characteristicに書き込まれたdata
 * @return Request - CTAPに沿ったRequest形式
 */
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

/**
 * @brief CTAPのコマンドに対応した関数を呼び出す 
 * 
 * @param request - CTAPに沿ったRequest形式
 * @return Response - CTAPに沿ったResponse形式
 */
Response ControlPointCallbacks::operateCTAPCommand(Request request) {
    Response response;
    
    switch (request.command) { /* Commandに応じた関数を呼び出す */
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

        default: /* Commandが存在しない場合 */
            throw implement_error("Not implement CTAP Command.");
    }
}

/**
 * @brief PINGコマンドの処理
 * 
 * @param request - CTAPに沿ったRequest形式
 * @return Response - PINGコマンドの返り値
 */
Response ControlPointCallbacks::parsePingCommand(Request request) {
    throw implement_error("Not implement PING Command.");
}

/**
 * @brief KEEPALIVEのコマンド処理
 * 
 * @param request - CTAPに沿ったRequest形式
 * @return Response - KEEPALIVEコマンドの返り値
 */
Response ControlPointCallbacks::parseKeepAliveCommand(Request request) {
    throw implement_error("Not implement KEEPALIVE Command.");
}

/**
 * @brief MSGコマンドの処理
 * 
 * @param request - CTAPに沿ったRequest形式
 * @return Response - MSGコマンドの返り値 
 */
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
    } catch (implement_error e) { /* 未実装コマンドだった場合 */
        throw implement_error(e.what());
    }

    return response;
}

/**
 * @brief CANCELコマンドの処理
 * 
 * @param request - CTAPに沿ったRequest形式
 * @return Response - CANCELコマンドの返り値
 */
Response ControlPointCallbacks::parseCancelCommand(Request request) {
    throw implement_error("Not implement CANCEL Command.");
}

/**
 * @brief ERRORコマンドの処理
 * 
 * @param request - CTAPに沿ったRequest形式
 * @return Response - ERRORコマンドの返り値
 */
Response ControlPointCallbacks::parseErrorCommand(Request request) {
    throw implement_error("Not implement ERROR Command.");
}

// エラー:DOMException: GATT Error: Not supported.
// TODO: BLE_Notifyのようにエラーが出ずNotifyできるようにする
// もしかするとEventListenerが問題かも？
void StatusCallbacks::onWrite(BLECharacteristic *characteristic) {
    characteristic->notify();
}