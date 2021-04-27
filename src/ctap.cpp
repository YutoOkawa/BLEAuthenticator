#include "ctap.hpp"

Request::~Request() {
    // Serial.println("Request destroy");
}

ContinuationFragments::~ContinuationFragments() {
    // Serial.println("ContinuationFragments destory");
}

/* ----------------------KeepAliveConstParam---------------------- */
/** @brief Status:PROCESSINGの識別子(0x01) */
const int KeepAliveConstParam::KEEPALIVE_PROCESSING = 0x01;

/** @brief Status:UP_NEEDEDの識別子(0x02) */
const int KeepAliveConstParam::KEEPALIVE_UP_NEEDED = 0x02;


/* ----------------------ErrorConstParam---------------------- */
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