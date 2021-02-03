#include "authAPI.hpp"

/** @brief authenticatorMakeCredentialのコマンド識別子(0x01) */
const int AuthenticatorAPICommandParam::COMMAND_MAKECREDENTIAL = 0x01;

/** @brief authenticatorGetAssertionのコマンド識別子(0x02) */
const int AuthenticatorAPICommandParam::COMMAND_GETASSERTION = 0x02;

/** @brief authenticatorGetInfoのコマンド識別子(0x04) */
const int AuthenticatorAPICommandParam::COMMAND_GETINFO = 0x04;

/** @brief authenticatorClientPINのコマンド識別子(0x06) */
const int AuthenticatorAPICommandParam::COMMAND_CLIENTPIN = 0x06;

/** @brief authenticatorResetのコマンド識別子(0x07) */
const int AuthenticatorAPICommandParam::COMMAND_RESET = 0x07;

/** @brief authenticatorGetNextAssertionのコマンド識別子(0x08) */
const int AuthenticatorAPICommandParam::COMMAND_GETNEXTASSERTION = 0x08;

/** @brief authenticatorVendorFirstのコマンド識別子(0x40) */
const int AuthenticatorAPICommandParam::COMMAND_VENDORFIRST = 0x40;

/** @brief authenticatorVendorLastのコマンド識別子(0xbf) */
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
 * @brief Commandに対応したAuthenticatorAPIを実行する
 * 
 * @return Response - AuthenticatorAPIの返り値
 */
Response AuthenticatorAPI::operateCommand() {
    switch(this->command) { /* Commandに応じた関数を呼び出す */
        case AuthenticatorAPICommandParam::COMMAND_MAKECREDENTIAL:
            return this->authenticatorMakeCredential();

        case AuthenticatorAPICommandParam::COMMAND_GETASSERTION:
            return this->authenticatorGetAssertion();

        case AuthenticatorAPICommandParam::COMMAND_GETINFO:
            return this->authenticatorGetInfo();

        case AuthenticatorAPICommandParam::COMMAND_CLIENTPIN:
            return this->authenticatorClientPIN();

        case AuthenticatorAPICommandParam::COMMAND_RESET:
            return this->authenticatorReset();

        case AuthenticatorAPICommandParam::COMMAND_GETNEXTASSERTION:
            return this->authenticatorGetNextAssertion();

        case AuthenticatorAPICommandParam::COMMAND_VENDORFIRST:
            return this->authenticatorVendorFirst();

        case AuthenticatorAPICommandParam::COMMAND_VENDORLAST:
            return this->authenciatorVendorLast();
            
        default: /* Commandが存在しない場合 */
            throw implement_error("This command isn't implemented.");
    }
}

/**
 * @brief authenticatorMakeCredential(0x01)を実行する
 * 
 * @return Response - authenticatorMakeCredentialに対応した返り値
 */
Response AuthenticatorAPI::authenticatorMakeCredential() {
    throw implement_error("Not implement MakeCredential Content.");
}

/**
 * @brief authenticatorGetAssertion(0x02)を実行する
 * 
 * @return Response - authenticatorGetAssertionに対応した返り値
 */
Response AuthenticatorAPI::authenticatorGetAssertion() {
    throw implement_error("Not implement GetAssertion Content.");
}

/**
 * @brief authenticatorGetInfo(0x04)を実行する
 * 
 * @return Response - authenticatorGetInfoに対応した返り値
 */
Response AuthenticatorAPI::authenticatorGetInfo() {
    throw implement_error("Not implement GetInfo Content.");
}

/**
 * @brief authenticatorClientPIN(0x06)を実行する
 * 
 * @return Response - authenticatorClientPINに対応した返り値
 */
Response AuthenticatorAPI::authenticatorClientPIN() {
    throw implement_error("Not implement ClientPIN Content.");
}

/**
 * @brief authenticatorReset(0x07)を実行する
 * 
 * @return Reponse - authenticatorResetに対応した返り値
 */
Response AuthenticatorAPI::authenticatorReset() {
    throw implement_error("Not implement Reset Content.");
}

/**
 * @brief authenticatorGetNextAssertion(0x08)を実行する
 * 
 * @return Response - authenticatorGetNextAssertionに対応した返り値
 */
Response AuthenticatorAPI::authenticatorGetNextAssertion() {
    throw implement_error("Not implement GetNextAssertion Content.");
}

/**
 * @brief authenticatorVendorFirst(0x40)を実行する
 * 
 * @return Response - authenticatorVendorFirstに対応した返り値
 */
Response AuthenticatorAPI::authenticatorVendorFirst() {
    throw implement_error("Not implement VendorFirst Content.");
}

/**
 * @brief authenticatorVendorLast(0xbf)を実行する
 * 
 * @return Response - authenticatorVendorLastに対応した返り値
 */
Response AuthenticatorAPI::authenciatorVendorLast() {
    throw implement_error("Not implement VendorLast Content.");
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