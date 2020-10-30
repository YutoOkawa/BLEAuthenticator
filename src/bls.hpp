#ifndef INCLUDED_bls_h_
#define INCLUDED_bls_h_

#include <BLEDevice.h>
#include <BLEDevice.h>
#include <BLEUtils.h>

#define SERVICE_UUID "0000fffd-0000-1000-8000-00805f9b34fb"
#define CHARACTERISTIC_CONTROLPOINT_UUID "f1d0fff1-deaa-ecee-b42f-c9ba7ed623bb"
#define CHARACTERISTIC_STATUS_UUID "f1d0fff2-deaa-ecee-b42f-c9ba7ed623bb"
#define CHARACTERISTIC_CONTROLPOINTLENGTH_UUID "f1d0fff3-deaa-ecee-b42f-c9ba7ed623bb"
#define CHARACTERISTIC_SERVICEREVISIONBITFIELD_UUID "f1d0fff4-deaa-ecee-b42f-c9ba7ed623bb"
#define CHARACTERISTIC_SERVICEREVISION_UUID "00002a28-0000-1000-8000-00805f9b34fb"

void initService();

#endif