#ifndef INCLUDED_message_h_
#define INCLUDED_message_h_

#include <stdint.h>

/**
 * @struct Command
 * @brief Message Encodingに従ったコマンド形式
 */
struct Command {
    unsigned int commandValue = 0x00;
    uint8_t *commandParameter;
};

/**
 * @struct Response
 * @brief Message Encodingに従ったレスポンス形式
 */
struct Response {
    unsigned int status = 0x00;
    uint8_t *responseData;
};

/**
 * @struct StatusCodeParam
 * @brief レスポンスのエラーコード
 */
struct StatusCodeParam {
    /**
     * @def CTAP1_ERR_SUCCESS;
     * @brief Indicates successful response. (0x00)
     */
    static const int CTAP1_ERR_SUCCESS;

    /**
     * @def CTAP1_ERR_INVALID_COMMAND
     * @brief The command is not a valid CTAP command. (0x01)
     */
    static const int CTAP1_ERR_INVALID_COMMAND;

    /**
     * @def CTAP1_ERR_INVALID_PARAMETER
     * @brief The command included an invalid parameter. (0x02)
     */
    static const int CTAP1_ERR_INVALID_PARAMETER;

    /**
     * @def CTAP1_ERR_INVALID_LENGTH
     * @brief Invalid message or item length. (0x03)
     */
    static const int CTAP1_ERR_INVALID_LENGTH;

    /**
     * @def CTAP1_ERR_INVALID_SEQ
     * @brief Invalid message sequencing. (0x04)
     */
    static const int CTAP1_ERR_INVALID_SEQ;

    /**
     * @def CTAP1_ERR_TIMEOUT
     * @brief Message timed out. (0x05)
     */
    static const int CTAP1_ERR_TIMEOUT;

    /**
     * @def CTAP1_ERR_CHANNEL_BUSY
     * @brief Channel busy. (0x06)
     */
    static const int CTAP1_ERR_CHANNEL_BUSY;

    /**
     * @def CTAP1_ERR_LOCK_REQUIRED
     * @brief Command requires channel lock. (0x0A)
     */
    static const int CTAP1_ERR_LOCK_REQUIRED;

    /**
     * @def CTAP1_ERR_INVALID_CHANNEL
     * @brief Command not allowed on this cid. (0x0B)
     */
    static const int CTAP1_ERR_INVALID_CHANNEL;

    /**
     * @def CTAP2_ERR_CBOR_UNEXPECTED_TYPE
     * @brief Invalid/unexpected CBOR error. (0x11)
     */
    static const int CTAP2_ERR_CBOR_UNEXPECTED_TYPE;

    /**
     * @def CTAP2_ERR_INVALID_CBOR
     * @brief Error when parsing CBOR. (0x12)
     */
    static const int CTAP2_ERR_INVALID_CBOR;

    /**
     * @def CTAP2_ERR_MISSING_PARAMETER
     * @brief Missing non-optional parameter. (0x14)
     */
    static const int CTAP2_ERR_MISSING_PARAMETER;

    /**
     * @def CTAP2_ERR_LIMIT_EXCEEDED
     * @brief Limit for number of items exceeded. (0x15)
     */
    static const int CTAP2_ERR_LIMIT_EXCEEDED;

    /**
     * @def CTAP2_ERR_UNSUPPORTED_EXTENSION
     * @brief Unsupported extension. (0x16)
     */
    static const int CTAP2_ERR_UNSUPPORTED_EXTENSION;

    /**
     * @def CTAP2_ERR_CREDENTIAL_EXCLUDED
     * @brief Valid credential found in the exclude list. (0x19)
     */
    static const int CTAP2_ERR_CREDENTIAL_EXCLUDED;

    /**
     * @def CTAP2_ERR_PROCESSING
     * @brief Processing (Lengthy operation is in progress). (0x21)
     */
    static const int CTAP2_ERR_PROCESSING;

    /**
     * @def CTAP2_ERR_INVALID_CREDENTIAL
     * @brief Credential not valid for the authenticator. (0x22)
     */
    static const int CTAP2_ERR_INVALID_CREDENTIAL;

    /**
     * @def CTAP2_ERR_USER_ACTION_PENDING
     * @brief Authentication is waiting for user interaction. (0x23)
     */
    static const int CTAP2_ERR_USER_ACTION_PENDING;

    /**
     * @def CTAP2_ERR_OPERATION_PENDING
     * @brief Processing, lengthy operation is in progress. (0x24)
     */
    static const int CTAP2_ERR_OPERATION_PENDING;

    /**
     * @def CTAP2_ERR_NO_OPERATIONS
     * @brief No request is pending. (0x25)
     */
    static const int CTAP2_ERR_NO_OPERATIONS;

    /**
     * @def CTAP2_ERR_UNSUPPORTED_ALGORITHM
     * @brief Authenticator does not support requested algorithm. (0x26)
     */
    static const int CTAP2_ERR_UNSUPPORTED_ALGORITHM;

    /**
     * @def CTAP2_ERR_OPERATION_DENIED
     * @brief Not authorized for requested operation. (0x27)
     */
    static const int CTAP2_ERR_OPERATION_DENIED;

    /**
     * @def CTAP2_ERR_KEY_STORE_FULL
     * @brief Internal key storage is full. (0x28)
     */
    static const int CTAP2_ERR_KEY_STORE_FULL;

    /**
     * @def CTAP2_ERR_NOT_BUSY
     * @brief Authenticator cannot cancel as it is not busy. (0x29)
     */
    static const int CTAP2_ERR_NOT_BUSY;

    /**
     * @def CTAP2_ERR_NO_OPERATION_PENDING
     * @brief No outstanding operations. (0x2A)
     */
    static const int CTAP2_ERR_NO_OPERATION_PENDING;

    /**
     * @def CTAP2_ERR_UNSUPPORTED_OPTION
     * @brief Unsupported option. (0x2B)
     */
    static const int CTAP2_ERR_UNSUPPORTED_OPTION;

    /**
     * @def CTAP2_ERR_INVALID_OPTION
     * @brief Not a valid option for current operation. (0x2C)
     */
    static const int CTAP2_ERR_INVALID_OPTION;

    /**
     * @def CTAP2_ERR_KEEPALIVE_CANCEL
     * @brief Pending keep alive was cancelled. (0x2D)
     */
    static const int CTAP2_ERR_KEEPALIVE_CANCEL;

    /**
     * @def CTAP2_ERR_NO_CREDENTIALS
     * @brief No valid credentials provided. (0x2E)
     */
    static const int CTAP2_ERR_NO_CREDENTIALS;

    /**
     * @def CTAP2_ERR_USER_ACTION_TIMEOUT
     * @brief Timeout waiting for user interaction. (0x2F)
     */
    static const int CTAP2_ERR_USER_ACTION_TIMEOUT;

    /**
     * @def CTAP2_ERR_NOT_ALLOWED
     * @brief Continuation command, such as, authenticatorGetNextAssertion not allowed. (0x30)
     */
    static const int CTAP2_ERR_NOT_ALLOWED;

    /**
     * @def CTAP2_ERR_PIN_INVALIED
     * @brief PIN Invalid. (0x31)
     */
    static const int CTAP2_ERR_PIN_INVALID;

    /**
     * @def CTAP2_ERR_PIN_BLOCKED
     * @brief PIN Blocked. (0x32)
     */
    static const int CTAP2_ERR_PIN_BLOCKED;

    /**
     * @def CTAP2_ERR_PIN_AUTH_INVALID
     * @brief PIN authentication,pinAuth, verification failed. (0x33)
     */
    static const int CTAP2_ERR_PIN_AUTH_INVALID;

    /**
     * @def CTAP2_ERR_PIN_AUTH_BLOCKED
     * @brief PIN authentication,pinAuth, blocked. Requires power recycle to reset. (0x34)
     */
    static const int CTAP2_ERR_PIN_AUTH_BLOCKED;

    /**
     * @def CTAP2_ERR_PIN_NOT_SET
     * @brief No PIN has been set. (0x35)
     */
    static const int CTAP2_ERR_PIN_NOT_SET;

    /**
     * @def CTAP2_ERR_PIN_REQUIRED
     * @brief PIN is required for the selected operation. (0x36)
     */
    static const int CTAP2_ERR_PIN_REQUIRED;

    /**
     * @def CTAP2_ERR_PIN_POLICY_VIOLATION
     * @brief PIN policy violation. Currently only enforces minimum length. (0x37)
     */
    static const int CTAP2_ERR_PIN_POLICY_VIOLATION;

    /**
     * @def CTAP2_ERR_PIN_TOKEN_EXPIRED
     * @brief pinToken expired on authenticator. (0x38)
     */
    static const int CTAP2_ERR_PIN_TOKEN_EXPIRED;

    /**
     * @def CTAP2_ERR_REQUEST_TOO_LARGE
     * @brief Authenticator cannot handle this request due to memory constraints. (0x39)
     */
    static const int CTAP2_ERR_REQUEST_TOO_LARGE;

    /**
     * @def CTAP2_ERR_ACTION_TIMEOUT
     * @brief The current operation has timed out. (0x3A)
     */
    static const int CTAP2_ERR_ACTION_TIMEOUT;

    /**
     * @def CTAP2_ERR_UP_REQUIRED
     * @brief User presence is required for the requested operation. (0x3B)
     */
    static const int CTAP2_ERR_UP_REQUIRED;

    /**
     * @def CTAP1_ERR_OTHER;
     * @brief Other unspecified error. (0x7F)
     */
    static const int CTAP1_ERR_OTHER;

    /**
     *  @def CTAP2_ERR_SPEC_LAST
     *  @brief CTAP 2 spec last error. (0xDF)
     */
    static const int CTAP2_ERR_SPEC_LAST;

    /**
     * @def CTAP2_ERR_EXTESTION_FIRST
     * @brief Extension specific error. (0xE0)
     */
    static const int CTAP2_ERR_EXTENSION_FIRST;

    /**
     * @def CTAP2_ERR_EXTENSTION_LAST
     * @brief Extension specific error. (0xEF)
     */
    static const int CTAP2_ERR_EXTENSTION_LAST;

    /**
     * @def CTAP2_ERR_VENDOR_FIRST
     * @brief Vendor specific error. (0xF0)
     */
    static const int CTAP2_ERR_VENDOR_FIRST;

    /**
     * @def CTAP2_ERR_VENDOR_LAST
     * @brief Vendor specific error. (0xFF)
     */
    static const int CTAP2_ERR_VENDOR_LAST;
};

#endif