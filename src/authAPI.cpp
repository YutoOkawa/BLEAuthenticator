#include "authAPI.hpp"

/* ----------------------AuthenticatorAPICommandParam---------------------- */
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


/* ----------------------DebugUtility---------------------- */
void responseSerialDebug(Response response, size_t data_len) {
    Serial.printf("Response Status:%x\n", response.status);
    for (size_t i=0; i < data_len; ++i) {
        Serial.printf("%.2x", response.responseData[i]);
    }
}


/* ----------------------AuthenticatorAPI---------------------- */
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
            return this->authenticatorMakeCredential(); break;

        case AuthenticatorAPICommandParam::COMMAND_GETASSERTION:
            return this->authenticatorGetAssertion(); break;

        case AuthenticatorAPICommandParam::COMMAND_GETINFO:
            return this->authenticatorGetInfo(); break;

        case AuthenticatorAPICommandParam::COMMAND_CLIENTPIN:
            return this->authenticatorClientPIN(); break;

        case AuthenticatorAPICommandParam::COMMAND_RESET:
            return this->authenticatorReset(); break;

        case AuthenticatorAPICommandParam::COMMAND_GETNEXTASSERTION:
            return this->authenticatorGetNextAssertion(); break;

        case AuthenticatorAPICommandParam::COMMAND_VENDORFIRST:
            return this->authenticatorVendorFirst(); break;

        case AuthenticatorAPICommandParam::COMMAND_VENDORLAST:
            return this->authenciatorVendorLast(); break;
            
        default: /* Commandが存在しない場合 */
            throw implement_error("This command isn't implemented."); break;
    }
}

/**
 * @brief authenticatorMakeCredential(0x01)を実行する
 * 
 * @return Response - authenticatorMakeCredentialに対応した返り値
 */
Response AuthenticatorAPI::authenticatorMakeCredential() {
    /**
     * authenticatorMakeCredential Response
     */
    Response response;

    /**
     * MemberName       Required?
     * -------------    ----------
     * authData:        Required
     * fmt:             Required
     * attStmt:         Required
     */
    CBORPair response_data;

    /**
     * MemberName           Data type           Required?
     * -------------        ----------          ----------
     * clientDataHash:      Byte Array          Required
     * rp:                  PubkeyCredEntity    Required 
     * users:               PubkeyCredEnity     Required
     * pubKeyCredParams:    CBOR Array          Required
     * excludeList          Seq of PubkeyDesc   Optional
     * extensions           CBOR map            Optional
     * options              Map of options      Optional
     * pinAuth              ByteArray           Optional
     * pinProtocol          Unsigned Int        Optional
     */
    CBOR data = CBOR(this->parameter, this->length, true);
    CBOR cbor_clientDataHash;
    CBOR cbor_rp;
    CBOR cbor_user;
    CBOR cbor_pubKeyCredParams;

    /* clientDataHash */
    uint8_t *hash;
    if (data[MakeCredentialParam::KEY_CLIENT_DATA_HASH].is_bytestring()) {
        cbor_clientDataHash = data[MakeCredentialParam::KEY_CLIENT_DATA_HASH];

        hash = new uint8_t[cbor_clientDataHash.get_bytestring_len()];
        cbor_clientDataHash.get_bytestring(hash);
        for (size_t i=0; i<cbor_clientDataHash.get_bytestring_len(); ++i) {
            Serial.printf("%.2x", hash[i]);
        }
        Serial.println("");
        delete[] hash;
    }

    /* rp */
    PublicKeyCredentialRpEntity *rp = new PublicKeyCredentialRpEntity;
    if (data[MakeCredentialParam::KEY_RP].is_pair()) {
        cbor_rp = data[MakeCredentialParam::KEY_RP];

        cbor_rp["id"].get_string(rp->id);
        Serial.print("rp.id:");
        Serial.println(rp->id);

        cbor_rp["name"].get_string(rp->name);
        Serial.print("rp.name:");
        Serial.println(rp->name);
    }
    delete rp;

    /* user */
    PublicKeyCredentialUserEntity *user = new PublicKeyCredentialUserEntity;
    if (data[MakeCredentialParam::KEY_USER].is_pair()) {
        cbor_user = data[MakeCredentialParam::KEY_USER];

        user->id = new uint8_t[cbor_user["id"].get_bytestring_len()];
        cbor_user["id"].get_bytestring(user->id);
        Serial.print("user.id:");
        for (size_t i=0; i<cbor_user["id"].get_bytestring_len(); ++i) {
            Serial.printf("%.2x", user->id[i]);
        }
        Serial.println("");
        delete[] user->id;

        cbor_user["name"].get_string(user->name);
        Serial.print("user.name:");
        Serial.println(user->name);

        cbor_user["displayName"].get_string(user->displayName);
        Serial.print("user.displayName:");
        Serial.println(user->displayName);
    }
    delete user;


    /* pubKeyCredParams */
    PubKeyCredParam *pubKeyCredParams;
    if (data[MakeCredentialParam::KEY_PUBKEY_CRED_PARAM].is_array()) {
        cbor_pubKeyCredParams = data[MakeCredentialParam::KEY_PUBKEY_CRED_PARAM];
        CBOR params = cbor_pubKeyCredParams[0];

        pubKeyCredParams = new PubKeyCredParam[1];
        pubKeyCredParams->alg = (int)params["alg"];
        Serial.printf("pubKeyParam[0].alg:%d\n", pubKeyCredParams->alg);

        params["type"].get_string(pubKeyCredParams->type);
        Serial.print("pubKeyParam[0].type:");
        Serial.println(pubKeyCredParams->type);
        
        delete[] pubKeyCredParams;
    }

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
    /**
     * authenticatorGetInfo Response
     */
    Response response;

    /**
     * MemberName       Required?
     * -------------    ----------
     * versions:        Required
     * extensions:      Optional 
     * aaguid:          Required
     * options:         Optional
     * maxMsgSize:      Optional
     * pinProtocols:    Optional
     */
    CBORPair response_data = CBORPair(100);

    /**
     *  MemberName: versions
     *  DataType: Sequence of String
     *  Required?: Required
     */
    CBORArray cbor_versions = CBORArray(20);
    const char *versions[2] = {"CTAP_ABS", "FIDO_ABS"};
    cbor_versions.append(versions, 2);
    response_data.append(GetInfoResponseParam::KEY_VERSIONS, cbor_versions);

    /**
     * MemberName: extensions
     * DataType: Sequence of String
     * Required?: Optional
     */
    CBORArray cbor_extensions = CBORArray(20);
    const char *extensions[1] = {"hmac-secret"};
    cbor_extensions.append(extensions, 1);
    response_data.append(GetInfoResponseParam::KEY_EXTENSIONS, cbor_extensions);

    /**
     * MemberName: aaguid
     * DataType: Byte String
     * Required?: Required
     */
    const uint8_t aaguid[] = {
        0xF8, 0xA0, 0x11, 0xF3, 0x8C, 0x0A, 0x4D, 
        0x15, 0x80, 0x06, 0x17, 0x11, 0x1F, 0x9E, 0xDC, 0x7D
    };
    CBOR cbor_aaguid = CBOR();
    cbor_aaguid.encode(aaguid, 16);
    response_data.append(GetInfoResponseParam::KEY_AAGUID, cbor_aaguid);


    /**
     * MemberName: options
     * DataType: Map
     * Required?: Optional
     */
    CBORPair cbor_options = CBORPair(100);
    cbor_options.append("plat", false);
    cbor_options.append("rk", false);
    cbor_options.append("clientPin", false);
    cbor_options.append("up", false);
    cbor_options.append("uv", false);
    response_data.append(GetInfoResponseParam::KEY_OPTIONS, cbor_options);


    /**
     * MemberName: maxMsgSize
     * DataType: Unsigned Integer
     * Required?: Optional
     */
    unsigned int maxMsgSize = 1200;
    response_data.append(GetInfoResponseParam::KEY_MAX_MSG_SIZE, maxMsgSize);


    /**
     * MemberName: pinProtocols
     * DataType: Array Of Unsigned Integers
     * Required?: Optional
     */
    CBORArray cbor_pinProtocols = CBORArray(20);
    const unsigned int pinProtocols[1] = {0x01};
    cbor_pinProtocols.append(pinProtocols, 1);
    response_data.append(GetInfoResponseParam::KEY_PIN_PROTOCOLS, cbor_pinProtocols);


    response.responseData = response_data.to_CBOR();
    responseSerialDebug(response, response_data.length());

    return response;

    // throw implement_error("Not implement GetInfo Content.");
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