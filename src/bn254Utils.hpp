#ifndef INCLUDED_bn254Utils_h_
#define INCLUDED_bn254Utils_h_

#include "pair_BN254.h"
#include "randapi.h"
#include <string.h>
#include <M5StickC.h>
#include "YACL.h"
#include "MsgPack.h"

using namespace core;
using namespace B256_28;
using namespace BN254;

/* -----Utility------ */
ECP getG1Element(csprng RNG);
ECP2 getG2Element(csprng RNG);
ECP *parseECPElement(CBOR cbor, String name);
ECP2 *parseECP2Element(CBOR cbor, String name);
BIG *createHash(char *msg);
BIG *convertInt(int number);

#endif