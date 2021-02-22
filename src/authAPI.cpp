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
/**
 * @brief responseのデバッグ情報出力(Serial)
 * 
 * @param response - responseデータ
 * @param data_len - responseDataの長さ
 */
void responseSerialDebug(Response response, size_t data_len) {
    Serial.printf("Response Status:%x\n", response.status);
    for (size_t i=0; i < data_len; ++i) {
        Serial.printf("%.2x", response.responseData[i]);
    }
}

/**
 * @brief Stringのデバッグ情報出力(Serial)
 * 
 * @param prop - prop表記
 * @param str - String本体
 */
void stringSerialDebug(String prop, String str) {
    Serial.print(prop);
    Serial.println(str);
}

/**
 * @brief uint8_tのデバッグ情報出力(Serial)
 * 
 * @param prop - prop表記
 * @param data - uint8_t本体
 * @param data_len - uint8_tの長さ
 */
void uint8SerialDebug(String prop, uint8_t *data, size_t data_len) {
    Serial.print(prop);
    for (size_t i=0; i < data_len; ++i) {
        Serial.printf("%.2x", data[i]);
    }
    Serial.println("");
}

/**
 * @brief intのデバッグ情報出力(Serial)
 * 
 * @param prop - prop表記
 * @param data - int本体
 */
void intSerialDebug(String prop, int data) {
    Serial.print(prop);
    Serial.println(data);
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
 *        Requestをパースしたのち各APIの機能を呼び出す
 * 
 * @return Response - AuthenticatorAPIの返り値
 */
Response AuthenticatorAPI::operateCommand() {
    Response response;

    if (this->command == AuthenticatorAPICommandParam::COMMAND_MAKECREDENTIAL) {
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
        ParsedMakeCredentialParams *params = new ParsedMakeCredentialParams;

        params->data = CBOR(this->parameter, this->length, true);

        /* clientDataHash */
        if (params->data[MakeCredentialParam::KEY_CLIENT_DATA_HASH].is_bytestring()) {
            params->cbor_clientDataHash = params->data[MakeCredentialParam::KEY_CLIENT_DATA_HASH];
            params->hash = new uint8_t[params->cbor_clientDataHash.get_bytestring_len()];
            params->cbor_clientDataHash.get_bytestring(params->hash);

            uint8SerialDebug("hash:", params->hash, params->cbor_clientDataHash.get_bytestring_len());
        }

        /* rp  */
        params->rp = new PublicKeyCredentialRpEntity;
        if (params->data[MakeCredentialParam::KEY_RP].is_pair()) {
            params->cbor_rp = params->data[MakeCredentialParam::KEY_RP];
            params->cbor_rp["id"].get_string(params->rp->id);
            params->cbor_rp["name"].get_string(params->rp->name);

            stringSerialDebug("rp.id:", params->rp->id);
            stringSerialDebug("rp.name:", params->rp->name);
        }

        /* user */
        params->user = new PublicKeyCredentialUserEntity;
        if (params->data[MakeCredentialParam::KEY_USER].is_pair()) {
            params->cbor_user = params->data[MakeCredentialParam::KEY_USER];
            params->user->id = new uint8_t[params->cbor_user["id"].get_bytestring_len()];
            params->cbor_user["id"].get_bytestring(params->user->id);
            params->cbor_user["name"].get_string(params->user->name);
            params->cbor_user["displayName"].get_string(params->user->displayName);

            uint8SerialDebug("user.id:", params->user->id, params->cbor_user["id"].get_bytestring_len());
            stringSerialDebug("user.name:", params->user->name);
            stringSerialDebug("user.displayName:", params->user->displayName);
        }

        /* pubKeyCredParams */
        /* TODO:pubKeyCredParamsのサイズの可変長化(2つ以上引数として求められる場合がある) */
        params->pubKeyCredParams = new PubKeyCredParam;
        if (params->data[MakeCredentialParam::KEY_PUBKEY_CRED_PARAM].is_array()) {
            params->cbor_pubKeyCredParams = params->data[MakeCredentialParam::KEY_PUBKEY_CRED_PARAM];
            CBOR pubKeyCredParam = params->cbor_pubKeyCredParams[0];
            params->pubKeyCredParams->alg = (int)pubKeyCredParam["alg"];
            pubKeyCredParam["type"].get_string(params->pubKeyCredParams->type);

            intSerialDebug("pubKeyCredParam.alg:", params->pubKeyCredParams->alg);
            stringSerialDebug("pubKeyCredParams.type:", params->pubKeyCredParams->type);
        }

        /* API呼び出し */
        response = this->authenticatorMakeCredential(params);

        /* delete */
        delete params->hash;
        delete params->rp;
        delete params->user->id;
        delete params->user;
        delete params->pubKeyCredParams;
        delete params;

        return response;

    } else if (this->command == AuthenticatorAPICommandParam::COMMAND_GETASSERTION) {
        return this->authenticatorGetAssertion();
    } else if (this->command == AuthenticatorAPICommandParam::COMMAND_GETINFO) {
        return this->authenticatorGetInfo();
    } else if (this->command == AuthenticatorAPICommandParam::COMMAND_CLIENTPIN) {
        return this->authenticatorClientPIN();
    } else if (this->command == AuthenticatorAPICommandParam::COMMAND_RESET) {
        return this->authenticatorReset();
    } else if (this->command == AuthenticatorAPICommandParam::COMMAND_GETNEXTASSERTION) {
        return this->authenticatorGetNextAssertion();
    } else if (this->command == AuthenticatorAPICommandParam::COMMAND_VENDORFIRST) {
        return this->authenticatorVendorFirst();
    } else if (this->command == AuthenticatorAPICommandParam::COMMAND_VENDORLAST) {
        return this->authenciatorVendorLast();
    } else { /* Commandが存在しない場合 */
        throw implement_error("This command isn't implemented.");
    }
}

/**
 * @brief authenticatorMakeCredential(0x01)を実行する
 * 
 * @return Response - authenticatorMakeCredentialに対応した返り値
 */
Response AuthenticatorAPI::authenticatorMakeCredential(ParsedMakeCredentialParams *params) {
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

    /* 1.excludeListにパラメータが存在し、認証器内部のデータと一致した場合はエラー */

    /* 2.pubKeyCredParamsにサポートしないCOSEのvalueがあればエラー */
    if (params->data[MakeCredentialParam::KEY_PUBKEY_CRED_PARAM].is_array()) {
        /* TODO:サポートするアルゴリズムの定数化 */
        switch (params->pubKeyCredParams->alg) {
            case -7:
                Serial.printf("Algorithm %d is Supported by this authenticator.\n", params->pubKeyCredParams->alg);
                break;
            default: /* 該当しないalgであればエラーを返す */
                Serial.println("This Algorithm is not Supported by this authenticator.");
                response.status = StatusCodeParam::CTAP2_ERR_UNSUPPORTED_ALGORITHM;
                return response;
        }
    }

    /* 3.optionのチェック */

    /* 4.extensionsのチェック */

    /* 5.pinAuthパラメータのチェック */

    /* 6.pinAuthがなくclientPinがセットされていればエラー */

    /* 7.pinAuthがありpinProtocolがサポートされていなければエラー */

    /* 8.ユーザのローカル認証を要求 */

    /* 9.アルゴリズムにしたがって鍵ペアを生成する */

    /* 10.optionsにrkが設定されている場合の処理 */

    /* 11.clientDataHashを使ってAttestation Statementを生成する */

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

    response.length = response_data.length();

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