#include "abs.hpp"

void test() {
    // // ECP->OCT->ECPの転写テスト
    // ECP test;
    // ECP_generator(&test);
    // Serial.print("ECP generator:");
    // ECP_output(&test);
    // char char_test[MODBYTES_B256_28+1];
    // octet octet_test = {0, sizeof(char_test), char_test};
    // ECP_toOctet(&octet_test, &test, true);
    // Serial.print("ECP generator OCT:");
    // OCT_output(&octet_test);
    // ECP test1;
    // ECP_fromOctet(&test1, &octet_test);
    // Serial.print("ECP generator OCT→ECP");
    // ECP_output(&test1);
}

/* ----------------------TPK---------------------- */
TPK::TPK() {

}

/**
 * @brief Construct a new TPK::TPK object
 * 
 * @param cbor_tpk tpkのCBOR形式
 */
TPK::TPK(CBOR cbor_tpk) {
    this->cbor_tpk = cbor_tpk;
}

TPK::~TPK() {

}

/**
 * @brief cborデータから鍵データをパースする
 */
void TPK::parse() {
    /* gのデータのパース(gのみ値がInfinityとなってしまう) */
    this->g = parseECPElement(this->cbor_tpk, "g");
    ECP_output(this->g);
}

/**
 * @brief getter
 * 
 * @return ECP Gを返す
 */
ECP *TPK::getG() {
    return this->g;
}

/**
 * @brief setter
 * 
 * @param g Gをセットする
 */
void TPK::setG(ECP *g) {
    this->g = g;
}

/* ----------------------APK---------------------- */
APK::APK() {

}

/**
 * @brief Construct a new APK::APK object
 * 
 * @param cbor_apk apkのCBOR形式
 */
APK::APK(CBOR cbor_apk) {
    this->cbor_apk = cbor_apk;
}

APK::~APK() {
    
}

/**
 * @brief cborデータから鍵データをパースする
 */
void APK::parse() {
    this->A0 = parseECP2Element(this->cbor_apk, "A0");
    ECP2_output(this->A0);
    this->C = parseECPElement(this->cbor_apk, "C");
    ECP_output(this->C);
}

/**
 * @brief getter
 * 
 * @return ECP2 A0を返す
 */
ECP2 *APK::getA0() {
    return this->A0;
}

/**
 * @brief getter
 * 
 * @return ECP Cを返す
 */
ECP *APK::getC() {
    return this->C;
}

void APK::setA0(ECP2 *A0) {
    this->A0 = A0;
}

void APK::setC(ECP *C) {
    this->C = C;
}

/* ----------------------SKA---------------------- */
SKA::SKA() {

}

SKA::SKA(CBOR cbor_ska) {
    this->cbor_ska = cbor_ska;
}

SKA::~SKA() {
    
}

/**
 * @brief CBORデータから鍵データをパースする
 * 
 */
void SKA::parse() {
    this->KBase = parseECPElement(this->cbor_ska, "KBase");
    ECP_output(this->KBase);
    this->K0 = parseECPElement(this->cbor_ska, "K0");
    ECP_output(this->K0);
}

ECP *SKA::getKBase() {
    return this->KBase;
}

ECP *SKA::getK0() {
    return this->K0;
}

void SKA::setKBase(ECP *KBase) {
    this->KBase = KBase;
}

void SKA::setK0(ECP *K0) {
    this->K0 = K0;
}

/* ----------------------Signature--------------------- */

/**
 * @brief 署名を生成する
 * 
 * @param tpk Trustee Public Key
 * @param apk Public Key
 * @param ska ユーザ秘密鍵
 * @param message 署名対象のメッセージ
 * @param policy ポリシー
 * @param RNG 乱数値
 * @return Signature 署名情報
 */
Signature sign(TPK tpk, APK apk, SKA ska, char *message, char *policy, csprng RNG) {
    char *hashmsg;
    int i;
    int msp[4][1] = {{0},{1},{1},{0}};
    BIG rd;
    // std::vector<BIG> rdlist;
    Signature signature;

    sprintf(hashmsg, "%s%s", *message, *policy);
    BIG *mu = createHash(hashmsg);
    for(int i=0; i<=sizeof(msp); i++) {
        BIG r;
        BIG_randtrunc(r, rd, 2 * CURVE_SECURITY_BN254, &RNG);
        // rdlist.push_back(r);
    }

    // Y = r[0] * KBase
    ECP Y;
    ECP_copy(&Y, ska.getKBase());
    // PAIR_G1mul(&Y, rdlist[0]);
    signature.setY(Y);

    // W = r[0] * K0
    ECP W;
    ECP_copy(&W, ska.getK0());
    // PAIR_G1mul(&W, rdlist[0]);
    signature.setW(W);

    return signature;
}