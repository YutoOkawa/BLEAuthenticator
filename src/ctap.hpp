#ifndef INCLUDED_ctap_h_
#define INCLUDED_ctap_h_

#include <BLEDevice.h>
#include <BLEDevice.h>
#include <BLEUtils.h>
#include <M5StickC.h>

struct CommandConstParam {
    static const int COMMAND_PING;
    static const int COMMAND_KEEPALIVE;
    static const int COMMAND_MSG;
    static const int COMMAND_CANCEL;
    static const int COMMAND_ERROR;
};

struct KeepAliveConstParam {
    static const int KEEPALIVE_PROCESSING;
    static const int KEEPALIVE_UP_NEEDED;
};

struct ErrorConstParam {
    static const int ERR_INVALID_CMD;
    static const int ERR_INVALID_PAR;
    static const int ERR_INVALID_LEN;
    static const int ERR_INVALID_SEQ;
    static const int ERR_REQ_TIMEOUT;
    static const int ERR_BUSY;
    static const int ERR_OTHER;
};

class ControlPointCallbacks: public BLECharacteristicCallbacks {
    private:
        uint8_t* data = new uint8_t[128];
    public:
        void onWrite(BLECharacteristic *characteristic);
};

#endif