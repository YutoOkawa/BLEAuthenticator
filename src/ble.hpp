#ifndef INCLUDED_ble_h_
#define INCLUDED_ble_h_

#include <BLEDevice.h>
#include <BLEUtils.h>
#include <BLE2902.h>
#include "ctap.hpp"

struct ServiceConstParam {
    static const char SERVICE_UUID[37];
};

struct CharacteristicConstParam {
    static const char CHARACTERISTIC_CONTROLPOINT_UUID[37];
    static const char CHARACTERISTIC_STATUS_UUID[37];
    static const char CHARACTERISTIC_CONTROLPOINTLENGTH_UUID[37];
    static const char CHARACTERISTIC_SERVICEREVISIONBITFIELD_UUID[37];
    static const char CHARACTERISTIC_SERVICEREVISION_UUID[37];
};

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

class ConnectServerCallbacks : public BLEServerCallbacks {
    private:
        bool connect = false;
    public:
        void onConnect(BLEServer *pServer);
        void onDisconnect(BLEServer *pServer);

        bool getConnect();
        void setConnect(bool connect);
};

class ControlPointCallbacks: public BLECharacteristicCallbacks {
    private:
        uint8_t* data;
        bool writeFlag = false;
        Response response;
        // size_t responseDataLength;
        // uint8_t *responseData;

    public:
        void onWrite(BLECharacteristic *characteristic);
        Request parseRequest(uint8_t *req);
        Response operateCTAPCommand(Request request);
        Response parsePingCommand(Request request);
        Response parseKeepAliveCommand(Request request);
        Response parseMsgCommand(Request request);
        Response parseCancelCommand(Request request);
        Response parseErrorCommand(Request request);

        bool getFlag();
        uint8_t *getResponseData();
        size_t getResponseDataLength();
        void setFlag(bool flag);
};

class StatusCallbacks: public BLECharacteristicCallbacks {
    public:
        void onNotify(BLECharacteristic *characteristic);
};


class CTAPBLE {
    private:
        BLEServer *pServer;
        BLEService *pService;
        /*
            * CharacteristicName - FIDO Control Point  
            * Property - Write
            * Length - Defined by Vendor(20-512bytes)
            * UUID - f1d0fff1-deaa-ecee-b42f-c9ba7ed623bb
        */
        BLECharacteristic *pCpCharacteristic;
        /*
            * CharacteristicName - FIDO Status
            * Property - Notify
            * Length - N/A
            * UUID - f1d0fff2-deaa-ecee-b42f-c9ba7ed623bb
        */
        BLECharacteristic *pStatusCharacteristic;
        /*
            * CharacteristicName - FIDO Control Point Length
            * Property - Read
            * Length - 2 bytes
            * UUID - f1d0fff3-deaa-ecee-b42f-c9ba7ed623bb
        */
        BLECharacteristic *pCpLengthCharacteristic;
        /*
            * CharacteristicName - FIDO Sercice Revision Bitfield
            * Property - Read/Write
            * Length - Definede by Vendor(20-512bytes)
            * UUID - f1d0fff4-deaa-ecee-b42f-c9ba7ed623bb
        */
        BLECharacteristic *pSrbBitCharacteristic;
        /*
            * CharacteristicName - FIDO Service Revision
            * Property - Read
            * Length - Defined by Vendor(20-512bytes)
            * UUID - 00002a28-0000-1000-8000-00805f9b34fb
        */
        BLECharacteristic *pSrbCharacteristic;
        BLEAdvertising *pAdvertising;

        ConnectServerCallbacks *connectServer;
        ControlPointCallbacks *controlPoint;
        StatusCallbacks *status;
        
    public:
        void init();
        void startService();
        void startAdvertise();
        // Setter
        void setServer(BLEServer *pServer);
        void setService(BLEService *pService);
        void setCpCharacteristic(BLECharacteristic *pCpCharacteristic);
        void setStatusCharacteristic(BLECharacteristic *pStatusCharacteritisc);
        void setCpLengthCharacteristic(BLECharacteristic *pCpLengthCharacteristic);
        void setSrbBitCharacteristic(BLECharacteristic *pSrbBitCharacteritisc);
        void setSrbCharacteristic(BLECharacteristic *pSrbCharacteritic);
        void setAdvertising(BLEAdvertising *pAdvertising);
        // Getter
        BLEServer *getServer();
        BLEService *getService();
        BLECharacteristic *getCpCharacteristic();
        BLECharacteristic *getStatusCharacteristic();
        BLECharacteristic *getCpLengthCharacteristic();
        BLECharacteristic *getSrbBitCharacteritsic();
        BLECharacteristic *getSrbCharacteritsic();
        BLEAdvertising *getAdvertising();
        ConnectServerCallbacks *getConnectServer();
        ControlPointCallbacks *getControlPoint();
        StatusCallbacks *getStatus();
};

#endif