#ifndef INCLUDED_ctap_h_
#define INCLUDED_ctap_h_

#include <BLEDevice.h>
#include <BLEDevice.h>
#include <BLEUtils.h>

#include "authAPI.hpp"

/**
 * @struct CommandConstParam
 * @brief  CTAP(BLE)のCommand形式 Request/Responseの先頭1byte
 */ 
struct CommandConstParam {
    static const int COMMAND_PING;
    static const int COMMAND_KEEPALIVE;
    static const int COMMAND_MSG;
    static const int COMMAND_CANCEL;
    static const int COMMAND_ERROR;
};

/**
 * @struct KeepAliveConstParam
 * @brief  KEEPALIVEが返すステータス変数
 */ 
struct KeepAliveConstParam {
    static const int KEEPALIVE_PROCESSING;
    static const int KEEPALIVE_UP_NEEDED;
};

/**
 * @struct ErrorConstParam
 * @brief  Errorの内容を示す変数
 */ 
struct ErrorConstParam {
    static const int ERR_INVALID_CMD;
    static const int ERR_INVALID_PAR;
    static const int ERR_INVALID_LEN;
    static const int ERR_INVALID_SEQ;
    static const int ERR_REQ_TIMEOUT;
    static const int ERR_BUSY;
    static const int ERR_OTHER;
};

/**
 * @struct Request
 * @brief  Centralから受け取るリクエスト形式
 */
struct Request {
    /**
     * @var   command
     * @brief Command identifier.
     *        Length:1
     */
    unsigned int command;

    /**
     * @var   hlen
     * @brief Hight part of data length.
     *        Length:1
     */
    unsigned int hlen;

    /**
     * @var   llen
     * @brief Low part of data length.
     *        Length:1
     */
    unsigned int llen;

    /**
     * @var   data
     * @brief Data.
     *        Length:s (s is equal to the length)
     */
    Command data;
};

class ControlPointCallbacks: public BLECharacteristicCallbacks {
    private:
        uint8_t* data;
    public:
        void onWrite(BLECharacteristic *characteristic);
        Request parseRequest(uint8_t *req);
        Response operateCTAPCommand(Request request);
        Response parsePingCommand(Request request);
        Response parseKeepAliveCommand(Request request);
        Response parseMsgCommand(Request request);
        Response parseCancelCommand(Request request);
        Response parseErrorCommand(Request request);
};

class StatusCallbacks: public BLECharacteristicCallbacks {
    public:
        void onWrite(BLECharacteristic *characteristic);
};

#endif