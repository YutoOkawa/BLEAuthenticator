#include "authAPI.hpp"

const int AuthenticatorAPICommandParam::COMMAND_MAKECREDENTIAL = 0x01;
const int AuthenticatorAPICommandParam::COMMAND_GETASSERTION = 0x02;
const int AuthenticatorAPICommandParam::COMMAND_GETINFO = 0x04;
const int AuthenticatorAPICommandParam::COMMAND_CLIENTPIN = 0x06;
const int AuthenticatorAPICommandParam::COMMAND_RESET = 0x07;
const int AuthenticatorAPICommandParam::COMMAND_GETNEXTASSERTION = 0x08;
const int AuthenticatorAPICommandParam::COMMAND_VENDORFIRST = 0x40;
const int AuthenticatorAPICommandParam::COMMAND_VENDORLAST = 0xbf;