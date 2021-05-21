#ifndef INCLUDED_abs_h_
#define INCLUDED_abs_h_

#include "bn254Utils.hpp"
// #include <list>
// #include <vector>
// #include <map>

class TPK {
    private:
        ECP *g;
        // std::list<ECP2> h;
        // std::map<std::string, int> attriblist;
        CBOR cbor_tpk;

    public:
        TPK();
        TPK(CBOR cbor_tpk);
        ~TPK();

        void parse();

        ECP *getG();
        // std::list<ECP2> getH();
        // std::map<std::string, int> getAttriblist();
        void setG(ECP *g);
        // void setH(std::list<ECP2> h);
        // void setAttriblist(std::map<std::string, int> attriblist);
};

class APK {
    private:
        ECP2 *A0;
        // std::list<ECP2> A;
        // std::list<ECP2> B;
        ECP *C;
        CBOR cbor_apk;

    public:
        APK();
        APK(CBOR cbor_apk);
        ~APK();

        void parse();
        
        ECP2 *getA0();
        // std::list<ECP2> getA();
        // std::list<ECP2> getB();
        ECP *getC();
        void setA0(ECP2 *A0);
        // void setA(std::list<ECP2> A);
        // void setB(std::list<ECP2> B);
        void setC(ECP *C);
};

class SKA {
    private:
        ECP *KBase;
        ECP *K0;
        // std::map<std::string, ECP> K;
        CBOR cbor_ska;

    public:
        SKA();
        SKA(CBOR cbor_ska);
        ~SKA();

        void parse();

        ECP *getKBase();
        ECP *getK0();
        // std::map<std::string, ECP> getK();
        void setKBase(ECP *KBase);
        void setK0(ECP *K0);
        // void setK(std::map<std::string, ECP> K);
};

class Signature {
    private:
        ECP Y;
        ECP W;
        // std::list<ECP> S;
        // std::list<ECP2> P;

    public:
        Signature();

        ECP getY();
        ECP getW();
        // std::list<ECP> getS();
        // std::list<ECP2> getP();
        void setY(ECP Y);
        void setW(ECP W);
        // void setS(std::list<ECP> S);
        // void setP(std::list<ECP2> P);
};

Signature sign(TPK tpk, APK apk, SKA ska, char *message, char *policy);

#endif