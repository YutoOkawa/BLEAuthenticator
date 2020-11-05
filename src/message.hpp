#ifndef INCLUDED_message_h_
#define INCLUDED_message_h_

#include <BLEDevice.h>
#include <BLEDevice.h>
#include <BLEUtils.h>

struct Command {
    unsigned int commandValue = 0x00;
    uint8_t *commandParameter;
};

#endif