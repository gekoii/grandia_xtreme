#include "common.h"
#include "field/n27.h"

void *memset(void *, s32, u32);

s16 *EvtMsgTileGridInit(s16 *p) {
    evtMsgTileGrid = (u16 *)p;
    memset(p, 0, 0x4B0);
    return p + 0x4B0;
}
