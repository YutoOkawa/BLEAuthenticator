#include "abs.hpp"

void test() {
    // // ECP->OCT->ECPの転写テスト
    ECP test;
    ECP_generator(&test);
    Serial.print("ECP generator:");
    ECP_output(&test);
    char char_test[MODBYTES_B256_28+1];
    octet octet_test = {0, sizeof(char_test), char_test};
    ECP_toOctet(&octet_test, &test, true);
    Serial.print("ECP generator OCT:");
    OCT_output(&octet_test);
    ECP test1;
    ECP_fromOctet(&test1, &octet_test);
    Serial.print("ECP generator OCT→ECP");
    ECP_output(&test1);
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

/**
 * @brief Destroy the TPK::TPK object
 */
TPK::~TPK() {

}

/**
 * @brief cborデータから鍵データをパースする
 */
void TPK::parse() {
    // Serial.printf("g:");
    this->g = parseECPElement(this->cbor_tpk, "g");
    // ECP_output(this->g);
    for (int i=0; i<=8; i++) {
        char *key = new char[2];
        sprintf(key, "h%d", i);
        // Serial.printf("%s:", key);
        this->h.push_back(*parseECP2Element(this->cbor_tpk, key));
    }
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
 * @brief getter
 * 
 * @return MsgPack::arr_t<ECP2>  hを返す
 */
MsgPack::arr_t<ECP2> TPK::getH() {
    return this->h;
}

/**
 * @brief getter
 * 
 * @return MsgPack::map_t<String, int> 属性集合を返す
 */
MsgPack::map_t<String, int> TPK::getAttriblist() {
    return this->attriblist;
}

/**
 * @brief getter
 * 
 * @return CBOR cbor_tpk
 */
CBOR TPK::getCBOR() {
    return this->cbor_tpk;
}

/**
 * @brief setter
 * 
 * @param g Gをセットする
 */
void TPK::setG(ECP *g) {
    this->g = g;
}

/**
 * @brief setter
 * 
 * @param h hをセットする
 */
void TPK::setH(MsgPack::arr_t<ECP2> h) {
    this->h = h;
}

/**
 * @brief setter
 * 
 * @param attriblist 属性集合をセットする 
 */
void TPK::setAttriblist(MsgPack::map_t<String, int> attriblist) {
    this->attriblist = attriblist;
}

/**
 * @brief setter
 * 
 * @param cbor_tpk cbor_tpk 
 */
void TPK::setCBOR(CBOR cbor_tpk) {
    this->cbor_tpk = cbor_tpk;
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

/**
 * @brief Destroy the APK::APK object
 */
APK::~APK() {
    
}

/**
 * @brief cborデータから鍵データをパースする
 */
void APK::parse() {
    this->A0 = parseECP2Element(this->cbor_apk, "A0");
    // ECP2_output(this->A0);
    for (int i=1; i<=4; i++) {
        char *Akey = new char[2];
        char *Bkey = new char[2];
        sprintf(Akey, "A%d", i);
        sprintf(Bkey, "B%d", i);
        // Serial.printf("%s:", Akey);
        this->A.push_back(*parseECP2Element(this->cbor_apk, Akey));
        // ECP2_output(&this->A[i-1]);
        // Serial.printf("%s:", Bkey);
        this->B.push_back(*parseECP2Element(this->cbor_apk, Bkey));
        // ECP2_output(&this->B[i-1]);
    }
    // Serial.printf("C:");
    this->C = parseECPElement(this->cbor_apk, "C");
    // ECP_output(this->C);
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
 * @return MsgPack::arr_t<ECP2> Aを返す
 */
MsgPack::arr_t<ECP2> APK::getA() {
    return this->A;
}

/**
 * @brief getter
 * 
 * @return MsgPack::arr_t<ECP2> Bを返す
 */
MsgPack::arr_t<ECP2> APK::getB() {
    return this->B;
}

/**
 * @brief getter
 * 
 * @return ECP Cを返す
 */
ECP *APK::getC() {
    return this->C;
}

/**
 * @brief getter
 * 
 * @return CBOR cbor_apk 
 */
CBOR APK::getCBOR() {
    return this->cbor_apk;
}

/**
 * @brief setter
 * 
 * @param A0 A0をセットする
 */
void APK::setA0(ECP2 *A0) {
    this->A0 = A0;
}

/**
 * @brief setter
 * 
 * @param A Aをセットする
 */
void APK::setA(MsgPack::arr_t<ECP2> A) {
    this->A = A;
}

/**
 * @brief setter
 * 
 * @param B Bをセットする
 */
void APK::setB(MsgPack::arr_t<ECP2> B) {
    this->B = B;
}

/**
 * @brief setter
 * 
 * @param C Cをセットする
 */
void APK::setC(ECP *C) {
    this->C = C;
}

/**
 * @brief setter
 * 
 * @param cbor_apk cbor_apkをセットする 
 */
void APK::setCBOR(CBOR cbor_apk) {
    this->cbor_apk = cbor_apk;
}

/* ----------------------SKA---------------------- */
SKA::SKA() {

}

/**
 * @brief Construct a new SKA::SKA object
 * 
 * @param cbor_ska skaのCBOR形式
 */
SKA::SKA(CBOR cbor_ska) {
    this->cbor_ska = cbor_ska;
}

/**
 * @brief Destroy the SKA::SKA object
 */
SKA::~SKA() {
    
}

/**
 * @brief CBORデータから鍵データをパースする
 * 
 */
void SKA::parse() {
    // Serial.printf("KBase:");
    this->KBase = parseECPElement(this->cbor_ska, "KBase");
    // ECP_output(this->KBase);
    // Serial.printf("K0:");
    this->K0 = parseECPElement(this->cbor_ska, "K0");
    // ECP_output(this->K0);
    this->K["K2"] = *parseECPElement(this->cbor_ska, "K2");
}

/**
 * @brief getter
 * 
 * @return ECP* KBaseを返す
 */
ECP *SKA::getKBase() {
    return this->KBase;
}

/**
 * @brief getter
 * 
 * @return ECP* K0を返す
 */
ECP *SKA::getK0() {
    return this->K0;
}

/**
 * @brief getter
 * 
 * @return MsgPack::map_t<String, ECP> Kを返す
 */
MsgPack::map_t<String, ECP> SKA::getK() {
    return this->K;
}

/**
 * @brief getter
 * 
 * @return CBOR cbor_ska
 */
CBOR SKA::getCBOR() {
    return this->cbor_ska;
}

/**
 * @brief setter
 * 
 * @param KBase KBaseをセットする
 */
void SKA::setKBase(ECP *KBase) {
    this->KBase = KBase;
}

/**
 * @brief setter
 * 
 * @param K0 K0をセットする
 */
void SKA::setK0(ECP *K0) {
    this->K0 = K0;
}

/**
 * @brief setter
 * 
 * @param cbor_ska cbor_skaをセットする 
 */
void SKA::setCBOR(CBOR cbor_ska) {
    this->cbor_ska = cbor_ska;
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