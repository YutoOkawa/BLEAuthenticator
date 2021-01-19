#include "authAPI.hpp"

const int AuthenticatorAPICommandParam::COMMAND_MAKECREDENTIAL = 0x01;
const int AuthenticatorAPICommandParam::COMMAND_GETASSERTION = 0x02;
const int AuthenticatorAPICommandParam::COMMAND_GETINFO = 0x04;
const int AuthenticatorAPICommandParam::COMMAND_CLIENTPIN = 0x06;
const int AuthenticatorAPICommandParam::COMMAND_RESET = 0x07;
const int AuthenticatorAPICommandParam::COMMAND_GETNEXTASSERTION = 0x08;
const int AuthenticatorAPICommandParam::COMMAND_VENDORFIRST = 0x40;
const int AuthenticatorAPICommandParam::COMMAND_VENDORLAST = 0xbf;

/**
 * @brief Construct a new AuthenticatorAPI::AuthenticatorAPI object
 * 
 */
AuthenticatorAPI::AuthenticatorAPI() {
    this->command = 0x00;
    this->parameter = 0;
    this->length = 0;
}

/**
 * @brief Construct a new AuthenticatorAPI:: AuthenticatorAPI object
 * 
 * @param command AuthenticatorAPIのコマンド識別子
 */
AuthenticatorAPI::AuthenticatorAPI(unsigned int command) {
    this->command = command;
    this->parameter = 0;
    this->length = 0;
}

/**
 * @brief Construct a new AuthenticatorAPI:: AuthenticatorAPI object
 * 
 * @param command AuthenticatorAPIのコマンド識別子
 * @param parameter AuthenticatorAPIのパラメータ
 * @param length AuthenticatorAPIのパラメータの長さ
 */
AuthenticatorAPI::AuthenticatorAPI(unsigned int command, uint8_t *parameter, unsigned int length) {
    this->command = command;
    this->parameter = parameter;
    this->length = length;
}

/**
 * @brief getter - command
 * 
 * @return unsigned int - AuthenticatorAPIのコマンド識別子
 */
unsigned int AuthenticatorAPI::getCommand() {
    return this->command;
}

/**
 * @brief getter - parameter
 * 
 * @return uint8_t* - AuthenticatorAPIのパラメータ
 */
uint8_t *AuthenticatorAPI::getParameter() {
    return this->parameter;
}

/**
 * @brief getter - length
 * 
 * @return unsigned int - AuthenticatorAPIのパラメータの長さ
 */
unsigned int AuthenticatorAPI::getLength() {
    return this->length;
}

/**
 * @brief setter - command
 * 
 * @param command 
 */
void AuthenticatorAPI::setCommand(unsigned int command) {
    this->command = command;
}

/**
 * @brief setter - parameter
 * 
 * @param parameter 
 */
void AuthenticatorAPI::setParameter(uint8_t *parameter) {
    this->parameter = parameter;
}

/**
 * @brief setter - length
 * 
 * @param length parameterの長さ
 */
void AuthenticatorAPI::setLength(unsigned int length) {
    this->length = length;
}

/**
 * @brief パラメータを必要とするコマンドを確認する
 * 
 * @param command AuthenticatorAPIのコマンド識別子
 * @return コマンドが必要な値であればtrue
 *         必要がない値であればfalse
 */
bool checkHasParameters(unsigned int command) {
    if (command == AuthenticatorAPICommandParam::COMMAND_MAKECREDENTIAL ||
    command == AuthenticatorAPICommandParam::COMMAND_GETASSERTION ||
    command == AuthenticatorAPICommandParam::COMMAND_CLIENTPIN) {
        return true;
    } else {
        return false;
    }
}