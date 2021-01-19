#ifndef INCLUDED_authAPI_h_
#define INCLUDED_authAPI_h_

#include "message.hpp"

/**
 * @struct AuthenticatorAPICommandParam
 * @brief AuthenticatorAPIのコマンド識別子
 */
struct AuthenticatorAPICommandParam {
    /**
     * @def COMMAND_MAKECREDENTIAL
     * @brief authenticatorMakeCredentialのコマンド識別子(0x01)
     */
    static const int COMMAND_MAKECREDENTIAL;

    /**
     * @def COMMAND_GETASSERTION
     * @brief authenticatorGetAssertionのコマンド識別子(0x02)
     */
    static const int COMMAND_GETASSERTION;

    /**
     * @def COMMAND_GETINFO
     * @brief authenticatorGetInfoのコマンド識別子(0x04)
     */
    static const int COMMAND_GETINFO;

    /**
     * @def COMMAND_CLIENTPIN
     * @brief authenticatorClientPINのコマンド識別子(0x06)
     */
    static const int COMMAND_CLIENTPIN;

    /**
     * @def COMMAND_RESET
     * @brief authenticatorResetのコマンド識別子(0x07)
     */
    static const int COMMAND_RESET;

    /**
     * @def COMMAND_GETNEXTASSERTION
     * @brief authenticatorGetNextAssertionのコマンド識別子(0x08)
     */
    static const int COMMAND_GETNEXTASSERTION;

    /**
     * @def COMMAND_VENDORFIRST
     * @brief authenticatorVendorFirstのコマンド識別子(0x40)
     */
    static const int COMMAND_VENDORFIRST;

    /**
     * @def COMMAND_VENDORLAST
     * @brief authenticatorVendorLastのコマンド識別子(0xbf)
     */
    static const int COMMAND_VENDORLAST;
};

/**
 *  @class AuthenticatorAPI
 *  @brief AuthenticatorAPIの各メソッドを実行するクラス
 */
class AuthenticatorAPI {
    private:
        /**
         * @var command
         * @brief AuthenticatorAPIのコマンド識別子
         */
        unsigned int command;

        /**
         * @var parameter
         * @brief AuthenticatorAPIの引数パラメータ
         */
        uint8_t *parameter;

        /**
         *  @var len
         *  @brief parameterの長さ(llen-hlen)
         */
        unsigned int length;

    public:
        AuthenticatorAPI();
        AuthenticatorAPI(unsigned int command);
        AuthenticatorAPI(unsigned int command, uint8_t *parameter, unsigned int length);
        ~AuthenticatorAPI() {};
        Response start();

        unsigned int getCommand();
        uint8_t *getParameter();
        unsigned int getLength();
        void setCommand(unsigned int command);
        void setParameter(uint8_t *parameter);
        void setLength(unsigned int length);
};

bool checkHasParameters(unsigned int command);

#endif