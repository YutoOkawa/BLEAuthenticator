#ifndef INCLUDED_ctap_h_
#define INCLUDED_ctap_h_

#include <BLEDevice.h>
#include <BLEDevice.h>
#include <BLEUtils.h>
#include <M5StickC.h>

#include "message.hpp"

/**
 * @struct CommandConstParam
 * @brief  CTAP(BLE)のCommand形式 Request/Responseの先頭1byte
 */ 
struct CommandConstParam {
    /**
     * @def   COMMAND_PING
     * @brief PINGコマンドの識別子(0x81) 
     */ 
    static const int COMMAND_PING;

    /**
     * @def   COMMAND_KEEPALIVE
     * @brief KEEPALIVEコマンドの識別子(0x82)
     */
    static const int COMMAND_KEEPALIVE;

    /**
     * @def   COMMAND_MSG
     * @brief MSGコマンドの識別子(0x83)
     */
    static const int COMMAND_MSG;

    /**
     * @def   COMMAND_CANCEL
     * @brief CANCELコマンドの識別子(0xbe)
     */
    static const int COMMAND_CANCEL;

    /**
     * @def   COMMAND_ERROR
     * @brief ERRORコマンドの識別子(0xbf)
     */
    static const int COMMAND_ERROR;
};

/**
 * @struct KeepAliveConstParam
 * @brief  KEEPALIVEが返すステータス変数
 */ 
struct KeepAliveConstParam {
    /**
     * @def   KEEPALIVE_PROCESSING
     * @brief Status:PROCESSINGの識別子(0x01)
     */
    static const int KEEPALIVE_PROCESSING;

    /**
     * @def   KEEPALIVE_UP_NEEDED
     * @brief Status:UP_NEEDEDの識別子(0x02)
     */
    static const int KEEPALIVE_UP_NEEDED;
};

/**
 * @struct ErrorConstParam
 * @brief  Errorの内容を示す変数
 */ 
struct ErrorConstParam {
    /**
     * @def   ERR_INVALIED_CMD
     * @brief The command in the request is unknown/invalid (0x01)
     */
    static const int ERR_INVALID_CMD;

    /**
     * @def   ERR_INVALIED_PAR
     * @brief the parameters of the command is/are invalid or missing (0x02)
     */
    static const int ERR_INVALID_PAR;

    /**
     * @def   ERR_INVALIED_LEN
     * @brief The length of the request is invalid (0x03)
     */
    static const int ERR_INVALID_LEN;

    /**
     * @def   ERR_INVALIED_SEQ
     * @brief The sequence number is invalid (0x04)
     */
    static const int ERR_INVALID_SEQ;

    /**
     * @def   ERR_INVALIED_TIMEOUT
     * @brief THe request timed out (0x05)
     */
    static const int ERR_REQ_TIMEOUT;

    /**
     * @def   ERR_BUSY
     * @brief The deviceis busy and can't accept commands at this time (0x06)
     */
    static const int ERR_BUSY;

    /**
     * @def   ERR_OTHER
     * @brief Other, unspecified error (0x7f)
     */
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
        uint8_t* data = new uint8_t[128];
    public:
        void onWrite(BLECharacteristic *characteristic);
        Request parseRequest(uint8_t *req);
};

#endif