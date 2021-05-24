#ifndef INCLUDED_abs_h_
#define INCLUDED_abs_h_

#include "bn254Utils.hpp"
// #include <list>
// #include <vector>
// #include <map>

class TPK {
    private:
        ECP *g;
        MsgPack::arr_t<ECP2> h;
        MsgPack::map_t<String, int> attriblist;
        CBOR cbor_tpk;

    public:
        TPK();
        TPK(CBOR cbor_tpk);
        ~TPK();

        void parse();

        ECP *getG();
        MsgPack::arr_t<ECP2> getH();
        MsgPack::map_t<String, int> getAttriblist();
        void setG(ECP *g);
        void setH(MsgPack::arr_t<ECP2> h);
        void setAttriblist(MsgPack::map_t<String, int> attriblist);
};

class APK {
    private:
        ECP2 *A0;
        MsgPack::arr_t<ECP2> A;
        MsgPack::arr_t<ECP2> B;
        ECP *C;
        CBOR cbor_apk;

    public:
        APK();
        APK(CBOR cbor_apk);
        ~APK();

        void parse();
        
        ECP2 *getA0();
        MsgPack::arr_t<ECP2> getA();
        MsgPack::arr_t<ECP2> getB();
        ECP *getC();
        void setA0(ECP2 *A0);
        void setA(MsgPack::arr_t<ECP2> A);
        void setB(MsgPack::arr_t<ECP2> B);
        void setC(ECP *C);
};

class SKA {
    private:
        ECP *KBase;
        ECP *K0;
        MsgPack::map_t<String, ECP> K;
        CBOR cbor_ska;

    public:
        SKA();
        SKA(CBOR cbor_ska);
        ~SKA();

        void parse();

        ECP *getKBase();
        ECP *getK0();
        MsgPack::map_t<String, ECP> getK();
        void setKBase(ECP *KBase);
        void setK0(ECP *K0);
        void setK(MsgPack::map_t<String, ECP> K);
};

class Signature {
    private:
        ECP Y;
        ECP W;
        MsgPack::arr_t<ECP> S;
        MsgPack::arr_t<ECP2> P;

    public:
        Signature();

        ECP getY();
        ECP getW();
        MsgPack::arr_t<ECP> getS();
        MsgPack::arr_t<ECP2> getP();
        void setY(ECP Y);
        void setW(ECP W);
        void setS(MsgPack::arr_t<ECP>);
        void setP(MsgPack::arr_t<ECP2> P);
};

Signature sign(TPK tpk, APK apk, SKA ska, char *message, char *policy);

#endif