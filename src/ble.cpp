#include "ble.hpp"

/* ----------------------ServiceConstParam---------------------- */
const char ServiceConstParam::SERVICE_UUID[37] = "0000fffd-0000-1000-8000-00805f9b34fa";

/* ----------------------CharacteristicConstParam---------------------- */
const char CharacteristicConstParam::CHARACTERISTIC_CONTROLPOINT_UUID[37] = "f1d0fff1-deaa-ecee-b42f-c9ba7ed623bb";
const char CharacteristicConstParam::CHARACTERISTIC_STATUS_UUID[37] = "f1d0fff2-deaa-ecee-b42f-c9ba7ed623bb";
const char CharacteristicConstParam::CHARACTERISTIC_CONTROLPOINTLENGTH_UUID[37] = "f1d0fff3-deaa-ecee-b42f-c9ba7ed623bb";
const char CharacteristicConstParam::CHARACTERISTIC_SERVICEREVISIONBITFIELD_UUID[37] = "f1d0fff4-deaa-ecee-b42f-c9ba7ed623bb";
const char CharacteristicConstParam::CHARACTERISTIC_SERVICEREVISION_UUID[37] = "00002a28-0000-1000-8000-00805f9b34fb";

/* ----------------------CommandConstParam---------------------- */
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

/* ----------------------CTAPBLE---------------------- */
void CTAPBLE::init() {
    // Create the BLE Device
    BLEDevice::init("FIDO ABS Authenticator");
    this->connectServer = new ConnectServerCallbacks();
    this->controlPoint = new ControlPointCallbacks();
    this->status = new StatusCallbacks();
}

void CTAPBLE::startService() {
    // Create the BLE Server
    pServer = BLEDevice::createServer();
    pServer->setCallbacks(this->connectServer);

    // Create the BLE Service
    pService = pServer->createService(ServiceConstParam::SERVICE_UUID);

    // Create the BLE Characteristic
    pCpCharacteristic = pService->createCharacteristic(
        CharacteristicConstParam::CHARACTERISTIC_CONTROLPOINT_UUID,
        BLECharacteristic::PROPERTY_WRITE
    );
    pCpCharacteristic->setCallbacks(this->controlPoint);

    pStatusCharacteristic = pService->createCharacteristic(
        CharacteristicConstParam::CHARACTERISTIC_STATUS_UUID,
        BLECharacteristic::PROPERTY_NOTIFY
    );
    pStatusCharacteristic->addDescriptor(new BLE2902());
    pStatusCharacteristic->setCallbacks(this->status);

    pCpLengthCharacteristic = pService->createCharacteristic(
        CharacteristicConstParam::CHARACTERISTIC_CONTROLPOINTLENGTH_UUID,
        BLECharacteristic::PROPERTY_READ
    );
    pCpLengthCharacteristic->setValue("0x9b");

    pSrbBitCharacteristic = pService->createCharacteristic(
        CharacteristicConstParam::CHARACTERISTIC_SERVICEREVISIONBITFIELD_UUID,
        BLECharacteristic::PROPERTY_READ | 
        BLECharacteristic::PROPERTY_WRITE
    );
    pSrbBitCharacteristic->setValue("0x20");

    pSrbCharacteristic = pService->createCharacteristic(
        CharacteristicConstParam::CHARACTERISTIC_SERVICEREVISION_UUID,
        BLECharacteristic::PROPERTY_READ
    );
    pSrbCharacteristic->setValue("NOT recommended.");

    // Start the Service
    pService->start();

}

void CTAPBLE::startAdvertise() {
    // Start Adveritising
    pAdvertising = BLEDevice::getAdvertising();
    pAdvertising->addServiceUUID(ServiceConstParam::SERVICE_UUID);
    pAdvertising->setScanResponse(true);
    pAdvertising->setMinPreferred(0x0);
    BLEDevice::startAdvertising();
}

void CTAPBLE::setServer(BLEServer *pServer) {
    this->pServer = pServer;
}

void CTAPBLE::setService(BLEService *pService) {
    this->pService = pService;
}

void CTAPBLE::setCpCharacteristic(BLECharacteristic *pCpCharacteristic) {
    this->pCpCharacteristic = pCpCharacteristic;
}

void CTAPBLE::setStatusCharacteristic(BLECharacteristic *pStatusCharacteristic) {
    this->pStatusCharacteristic = pStatusCharacteristic;
}

void CTAPBLE::setCpLengthCharacteristic(BLECharacteristic *pCpLengthCharacteristic) {
    this->pCpLengthCharacteristic = pCpLengthCharacteristic;
}

void CTAPBLE::setSrbBitCharacteristic(BLECharacteristic *pSrbBitCharacteristic) {
    this->pSrbBitCharacteristic = pSrbBitCharacteristic;
}

void CTAPBLE::setSrbCharacteristic(BLECharacteristic *pSrbCharacteristic) {
    this->pSrbCharacteristic = pSrbCharacteristic;
}

void CTAPBLE::setAdvertising(BLEAdvertising *pAdvertising) {
    this->pAdvertising = pAdvertising;
}

BLEServer *CTAPBLE::getServer() {
    return this->pServer;
}

BLEService *CTAPBLE::getService() {
    return this->pService;
}

BLECharacteristic *CTAPBLE::getCpCharacteristic() {
    return this->pCpCharacteristic;
}

BLECharacteristic *CTAPBLE::getStatusCharacteristic() {
    return this->pStatusCharacteristic;
}

BLECharacteristic *CTAPBLE::getCpLengthCharacteristic() {
    return this->pCpLengthCharacteristic;
}

BLECharacteristic *CTAPBLE::getSrbBitCharacteritsic() {
    return this->pSrbBitCharacteristic;
}

BLECharacteristic *CTAPBLE::getSrbCharacteritsic() {
    return this->pSrbCharacteristic;
}

BLEAdvertising *CTAPBLE::getAdvertising() {
    return this->pAdvertising;
}

ConnectServerCallbacks *CTAPBLE::getConnectServer() {
    return this->connectServer;
}

ControlPointCallbacks *CTAPBLE::getControlPoint() {
    return this->controlPoint;
}

StatusCallbacks *CTAPBLE::getStatus() {
    return this->status;
}

/* ----------------------ConnectServerCallbacks---------------------- */
void ConnectServerCallbacks::onConnect(BLEServer *pServer) {
    this->connect = true;
    BLEDevice::startAdvertising();
}

void ConnectServerCallbacks::onDisconnect(BLEServer *pServer) {
    this->connect = false;
}

bool ConnectServerCallbacks::getConnect() {
    return this->connect;
}

void ConnectServerCallbacks::setConnect(bool connect) {
    this->connect = connect;
}

/* ----------------------ControlPointCallbacks---------------------- */
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

    // TODO:CBORデータの作成
    // uint8_t化とそのデータサイズの補完
    responseData = new uint8_t[response.length];
    memcpy(responseData, response.responseData, response.length);
    responseDataLength = response.length;

    // 書き込みが終わればフラグを立てる
    this->writeFlag = true;
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
            response = parsePingCommand(request); break;

        case CommandConstParam::COMMAND_KEEPALIVE:
            response = parseKeepAliveCommand(request); break;

        case CommandConstParam::COMMAND_MSG:
            response = parseMsgCommand(request); break;

        case CommandConstParam::COMMAND_CANCEL:
            response = parseCancelCommand(request); break;

        case CommandConstParam::COMMAND_ERROR:
            response = parseErrorCommand(request); break;

        default: /* Commandが存在しない場合 */
            throw implement_error("Not implement CTAP Command."); break;
    }

    return response;
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

uint8_t *ControlPointCallbacks::getResponseData() {
    return this->responseData;
}

size_t ControlPointCallbacks::getResponseDataLength() {
    return this->responseDataLength;
}

bool ControlPointCallbacks::getFlag() {
    return this->writeFlag;
}

void ControlPointCallbacks::setFlag(bool flag) {
    this->writeFlag = flag;
}

/* ----------------------StatusCallbacks---------------------- */
void StatusCallbacks::onNotify(BLECharacteristic *characteristic) {
    Serial.println("NOTIFY!");
}