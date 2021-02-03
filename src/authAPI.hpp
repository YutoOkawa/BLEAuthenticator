#ifndef INCLUDED_authAPI_h_
#define INCLUDED_authAPI_h_

#include "message.hpp"
#include <exception>

/**
 * @struct AuthenticatorAPICommandParam
 * @brief AuthenticatorAPIのコマンド識別子
 */
struct AuthenticatorAPICommandParam {
    static const int COMMAND_MAKECREDENTIAL;
    static const int COMMAND_GETASSERTION;
    static const int COMMAND_GETINFO;
    static const int COMMAND_CLIENTPIN;
    static const int COMMAND_RESET;
    static const int COMMAND_GETNEXTASSERTION;
    static const int COMMAND_VENDORFIRST;
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
        Response operateCommand();
        Response authenticatorMakeCredential();
        Response authenticatorGetAssertion();
        Response authenticatorGetInfo();
        Response authenticatorClientPIN();
        Response authenticatorReset();
        Response authenticatorGetNextAssertion();
        Response authenticatorVendorFirst();
        Response authenciatorVendorLast();

        unsigned int getCommand();
        uint8_t *getParameter();
        unsigned int getLength();
        void setCommand(unsigned int command);
        void setParameter(uint8_t *parameter);
        void setLength(unsigned int length);
};

bool checkHasParameters(unsigned int command);

/**
 * @class implement_error
 * @brief 未実装のエラー定義
 */
class implement_error : public std::exception {
    private:
        const char *msg;
    public:
        implement_error(const char* msg) {
            this->msg = msg;
        };
        const char* what() {
            return this->msg;
        };
};

#endif