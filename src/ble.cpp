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
CTAPBLE::~CTAPBLE() {
    delete this->connectServer;
    delete this->controlPoint;
    delete this->status;
}

/**
 * @brief BLE initialize function.
 */
void CTAPBLE::init() {
    // Create the BLE Device
    BLEDevice::init("FIDO ABS Authenticator");
    this->connectServer = new ConnectServerCallbacks();
    this->controlPoint = new ControlPointCallbacks();
    this->status = new StatusCallbacks();
}

/**
 * @brief BLE start service function.
 */
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

/**
 * @brief BLE start advertising.
 */
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
/**
 * @brief connectしたときに発火
 * 
 * @param pServer - connected server.
 */
void ConnectServerCallbacks::onConnect(BLEServer *pServer) {
    this->connect = true;
    BLEDevice::startAdvertising();
}

/**
 * @brief 接続解除したときに発火
 * 
 * @param pServer - disconnected server.
 */
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
ControlPointCallbacks::~ControlPointCallbacks() {
    delete authAPI;
}

/**
 * @brief characteristicに書き込まれたときに発火
 * 
 * @param characteristic - 書き込まれたcharacteristic
 */
void ControlPointCallbacks::onWrite(BLECharacteristic *characteristic) {
    // Request req;
    // Response res;

    this->data = characteristic->getData();
    this->request = parseRequest();
    try {
        this->response = operateCTAPCommand(this->request);
    } catch (implement_error e) { /* 未実装コマンドだった場合 */
        Serial.println(e.what());
    }

    Serial.println("");
    for (size_t i=0; i < response.length; ++i) {
        Serial.printf("%.2x", response.responseData[i]);
    }
    Serial.println("");

    // 書き込みが終わればフラグを立てる
    this->writeFlag = true;

    // TODO:Request, Response, authAPIのメモリ解放
    // Callbackの内部で保持しているため、デストラクタの呼び出しでdeleteするのが難しい
    // onWriteが終わったタイミングでデストラクタが呼び出されてそれぞれが解放されるのがベスト
    // この3つをまとめた構造体を作ってデストラクタで解放するか
    delete[] request.data.commandParameter;
    Serial.println("on Write End.");
}

// TODO: パース関数のエラー処理
/**
 * @brief dataをパースし、Requestを返す
 * 
 * @param req - characteristicに書き込まれたdata
 * @return Request - CTAPに沿ったRequest形式
 */
Request ControlPointCallbacks::parseRequest() {
    // TODO: commandがなかった場合のエラー処理
    // Commandを取得
    this->request.command = (unsigned int)*this->data;
    this->data++;

    // TODO: HLENがなかった場合のエラー処理
    // HLENを取得
    this->request.hlen = (unsigned int)*this->data;
    this->data++;

    // TODO: LLENがなかった場合のエラー処理
    // LLENを取得
    this->request.llen = (unsigned int)*this->data;
    this->data++;

    // TODO: Dataがなかった場合のエラー処理
    // Dataを取得
    // Command Valueを取得
    this->request.data.commandValue = (unsigned int)*this->data;
    this->data++;
    // Command Parameterを取得
    this->request.data.commandParameter = new uint8_t[this->request.llen-1];
    memcpy(this->request.data.commandParameter, this->data, this->request.llen-1);

    Serial.println("parseRequest End.");
    return this->request;
}

/**
 * @brief CTAPのコマンドに対応した関数を呼び出す 
 * 
 * @return Response - CTAPに沿ったResponse形式
 */
Response ControlPointCallbacks::operateCTAPCommand(Request request) {
    switch (request.command) { /* Commandに応じた関数を呼び出す */
        case CommandConstParam::COMMAND_PING:
            this->response = parsePingCommand(); break;

        case CommandConstParam::COMMAND_KEEPALIVE:
            this->response = parseKeepAliveCommand(); break;

        case CommandConstParam::COMMAND_MSG:
            this->response = parseMsgCommand(); break;

        case CommandConstParam::COMMAND_CANCEL:
            this->response = parseCancelCommand(); break;

        case CommandConstParam::COMMAND_ERROR:
            this->response = parseErrorCommand(); break;

        default: /* Commandが存在しない場合 */
            throw implement_error("Not implement CTAP Command."); break;
    }

    Serial.println("OperateCommand End.");
    return this->response;
}

/**
 * @brief PINGコマンドの処理
 * 
 * @return Response - PINGコマンドの返り値
 */
Response ControlPointCallbacks::parsePingCommand() {
    throw implement_error("Not implement PING Command.");
}

/**
 * @brief KEEPALIVEのコマンド処理
 * 
 * @return Response - KEEPALIVEコマンドの返り値
 */
Response ControlPointCallbacks::parseKeepAliveCommand() {
    throw implement_error("Not implement KEEPALIVE Command.");
}

/**
 * @brief MSGコマンドの処理
 * 
 * @return Response - MSGコマンドの返り値 
 */
Response ControlPointCallbacks::parseMsgCommand() {
    if (checkHasParameters(this->request.data.commandValue)) { /* パラメータを必要とするもの */
        this->authAPI = new AuthenticatorAPI(this->request.data.commandValue, this->request.data.commandParameter, this->request.llen - this->request.hlen - 1);
    } else { /* パラメータを必要としなもの */
        this->authAPI = new AuthenticatorAPI(this->request.data.commandValue);
    }

    try {
        this->response = this->authAPI->operateCommand();
    } catch (implement_error e) { /* 未実装コマンドだった場合 */
        throw implement_error(e.what());
    }

    Serial.println("parseMsgCommand End.");

    return this->response;
}

/**
 * @brief CANCELコマンドの処理
 * 
 * @return Response - CANCELコマンドの返り値
 */
Response ControlPointCallbacks::parseCancelCommand() {
    throw implement_error("Not implement CANCEL Command.");
}

/**
 * @brief ERRORコマンドの処理
 * 
 * @return Response - ERRORコマンドの返り値
 */
Response ControlPointCallbacks::parseErrorCommand() {
    throw implement_error("Not implement ERROR Command.");
}

uint8_t *ControlPointCallbacks::getResponseData() {
    return (uint8_t*)this->response.responseData;
}

size_t ControlPointCallbacks::getResponseDataLength() {
    return this->response.length;
}

bool ControlPointCallbacks::getFlag() {
    return this->writeFlag;
}

void ControlPointCallbacks::setFlag(bool flag) {
    this->writeFlag = flag;
}

/* ----------------------StatusCallbacks---------------------- */
/**
 * @brief notifyを送信したときに発火する
 * 
 * @param characteristic - Notifyしたcharacteristic
 */
void StatusCallbacks::onNotify(BLECharacteristic *characteristic) {
    Serial.println("NOTIFY!");
}