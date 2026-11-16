#include "common.h"
#include "save.h"
#include "protos/field.h"
#include "field/n16.h"

void EvtMapTreasureFlagSet(u16 arg0, u8 arg1) {
    s32 i = EvtMapFlagSlot(evtCurMapNo);

    if (i != -1) {
        evtMapFlagTable[i].treasureFlags[arg0 >> 3] &= ~(1 << (arg0 & 7));
        evtMapFlagTable[i].treasureFlags[arg0 >> 3] |= (arg1 & 1) << (arg0 & 7);
    }
}
