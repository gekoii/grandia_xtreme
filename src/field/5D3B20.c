#include "common.h"
#include "save.h"
#include "protos/field.h"
#include "field/n16.h"

s32 EvtMapFlagGet(u16 arg0) {
    s32 i = EvtMapFlagSlot(evtCurMapNo);

    if (i == -1) {
        return 0;
    }
    return (evtMapFlagTable[i].flags[arg0 >> 3] >> (arg0 & 7)) & 1;
}
