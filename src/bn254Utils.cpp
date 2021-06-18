#include "bn254Utils.hpp"

/**
 * @brief G1の要素をランダムに返す
 * 
 * @param RNG 乱数値
 * @return ECP ランダムなG1の要素
 */
ECP getG1Element(csprng RNG) {
    FP fp;
    ECP G1;

    FP_rand(&fp, &RNG);
    ECP_map2point(&G1, &fp);
    return G1;
}

/**
 * @brief G2の要素をランダムに返す
 * 
 * @param RNG 乱数値
 * @return ECP2 ランダムなG2の要素
 */
ECP2 getG2Element(csprng RNG) {
    FP2 fp2;
    ECP2 G2;

    FP2_rand(&fp2, &RNG);
    ECP2_map2point(&G2, &fp2);
    return G2;
}

/**
 * @brief CBORデータからECPデータを抽出する
 * 
 * @param cbor CBORデータ
 * @param key ECPデータのポインタ
 */
ECP *parseECPElement(CBOR cbor, String name) {
    ECP *key = new ECP();
    uint8_t *keyBytes_hex = new uint8_t[cbor[name.c_str()].get_bytestring_len()];
    char *keyBytes = new char[cbor[name.c_str()].get_bytestring_len()];
    octet KEYBYTES = {0, cbor[name.c_str()].get_bytestring_len(), keyBytes};

    // Serial.println("CBOR->uint8_t");
    cbor[name.c_str()].get_bytestring(keyBytes_hex); /* cbor->uint8_t */
    for (int i=0; i<cbor[name.c_str()].get_bytestring_len(); i++) { /* uint8_t->char */
        keyBytes[i] = keyBytes_hex[i];
    }
    // Serial.println("char->OCT");
    OCT_jstring(&KEYBYTES, keyBytes); /* char->OCT */
    // OCT_output(&KEYBYTES);
    // Serial.println("OCT->ECP");
    ECP_fromOctet(key, &KEYBYTES); /* OCT->ECP */
    // Serial.println("ECP Member Check.");
    if (!PAIR_G1member(key)) { /* ECPの内容がInfinityの場合 */
        Serial.printf("%s(ECP):No Member.\n", name.c_str());
        Serial.print("OCT:");
        OCT_output(&KEYBYTES);
        Serial.printf("ECP:");
        ECP_output(key);
    }
    // Serial.println("delete.");
    delete keyBytes_hex;
    delete keyBytes;
    return key;
}

/**
 * @brief CBORデータからECP2データを抽出する
 * 
 * @param cbor CBORデータ
 * @param key ECP2のデータのポインタ
 */
ECP2 *parseECP2Element(CBOR cbor, String name) {
    ECP2 *key = new ECP2();
    uint8_t *keyBytes_hex = new uint8_t[cbor[name.c_str()].get_bytestring_len()];
    char *keyBytes = new char[cbor[name.c_str()].get_bytestring_len()];
    octet KEYBYTES = {0, cbor[name.c_str()].get_bytestring_len(), keyBytes};

    // Serial.println("CBOR->uint8_t");
    cbor[name.c_str()].get_bytestring(keyBytes_hex); /* cbor->uint8_t */
    for (int i=0; i<cbor[name.c_str()].get_bytestring_len(); i++) { /* uint8_t->char */
        keyBytes[i] = keyBytes_hex[i];
    }
    // Serial.println("char->OCT");
    OCT_jstring(&KEYBYTES, keyBytes); /* char->OCT */
    // Serial.print("OCT:");
    // OCT_output(&KEYBYTES);
    // Serial.println("OCT->ECP2");
    ECP2_fromOctet(key, &KEYBYTES); /* OCT->ECP2 */
    /* なぜかエラーで落ちる */
    // Serial.println("ECP2 Member Check.");
    // if (!PAIR_G2member(key)) { /* ECP2の内容がInfinityの場合 */
    //     Serial.printf("%s(ECP2):No Member.\n", name.c_str());
    //     Serial.print("OCT:");
    //     OCT_output(&KEYBYTES);
    //     Serial.printf("ECP:");
    //     ECP2_output(key);
    // }
    // Serial.println("delete.");
    delete keyBytes_hex;
    delete keyBytes;
    return key;
}

/**
 * @brief ECPデータをバイトデータ(uint8_t)に変換する
 * 
 * @param g ECPデータ
 * @return uint8_t* ECPのバイトデータ
 */
uint8_t *ECPtoBuffer(ECP *g) {
    uint8_t *ecp = new uint8_t[MODBYTES_B256_28+1];
    char buf[MODBYTES_B256_28+1];
    octet BUF = {0, sizeof(buf), buf};
    ECP_toOctet(&BUF, g, true);
    memcpy(ecp, buf, MODBYTES_B256_28+1);
    return ecp;
}

uint8_t *ECP2toBuffer(ECP2 *h) {
    uint8_t *ecp2 = new uint8_t[2*MODBYTES_B256_28+1];
    char buf[2*(MODBYTES_B256_28+1)+1];
    octet BUF = {0, sizeof(buf), buf};
    ECP2_toOctet(&BUF, h, true);
    memcpy(ecp2, buf, 2*(MODBYTES_B256_28+1)+1);
    return ecp2;
}

/**
 * @brief CBORPairに署名情報を追加する
 * 
 * @param pair CBORPairのポインタ
 * @param g ECPデータ
 * @param key キーの名前
 */
void setECPSignature(CBORPair *pair, ECP *g, String key) {
    uint8_t *buf = ECPtoBuffer(g);
    CBOR G = CBOR();
    G.encode(buf, MODBYTES_B256_28+1); //Sのときここで落ちる
    pair->append(key.c_str(), G);
    delete buf;
}

void setECP2Signature(CBORPair *pair, ECP2 *h, String key) {
    uint8_t *buf = ECP2toBuffer(h);
    CBOR H = CBOR();
    H.encode(buf, 2*(MODBYTES_B256_28+1)+1);
    pair->append(key.c_str(), H);
}

/**
 * @brief Create a Hash object
 * 
 * @param msg メッセージ
 * @return BIG* ハッシュ化されたメッセージ
 */
void createHash(char *msg, BIG *mu) {
    hash512 sh512;
    int i;
    char digest[100];

    HASH512_init(&sh512);
    for(i=0; i<strlen(msg); i++) {
        HASH512_process(&sh512, msg[i]);
    }
    HASH512_hash(&sh512, digest);
    BIG_fromBytes(*mu, digest);
}

/**
 * @brief Int型をBIG型に変換する
 * 
 * @param number 数字
 * @return BIG* BIG型の数字表現
 */
BIG *convertInt(int number) {
    BIG one, *output;
    int i;

    BIG_one(one);
    BIG_one(*output);

    for(i=0; i<number; i++) {
        BIG_add(*output, *output, one);
    }
    return output;
}