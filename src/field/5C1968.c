#include "common.h"
#include "protos/field.h"
#include "m2c_macros.h"


s32 ChrTenantPlaySe(s16 arg0, s32 arg1) {
    if ((EvtSystemFlagGet(0x73) == 0) && (EvtSystemFlagGet(0x71) == 0) && (arg0 != 0)) {
        FldSndSePlay(0, arg0, arg1);
    }
    return 0;
}
