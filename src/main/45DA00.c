#include "common.h"
#include "protos/main.h"

extern u8 vuMicroCode0[];
extern u8 vuMicroCode1[];

void VuMicroLoadVu0(void) {
    GsVif0DmaSendN(vuMicroCode0, (u32)(vuMicroCode1 - vuMicroCode0) / 16);
}
