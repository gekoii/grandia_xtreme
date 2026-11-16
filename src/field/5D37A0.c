#include "common.h"
#include "save.h"
#include "protos/field.h"
#include "field/n16.h"

void MemSet(void *, s32, s32);

void EvtAreaStateReset(void) {
    s32 i;
    u8 *p;

    EvtSystemFlagSet(0x84, 0);
    EvtSystemFlagSet(0xFA, 0);
    EvtSystemFlagSet(0x65, 0);
    p = evtDungeonFlags;
    MemSet(p, 0, 8);
    MemSet(p + 8, 0, 0x20);
    MemSet(p - 0x26, 0, 0x10);
    MemSet(evtMapFlagTable, 0, 0x90);
    PtyClearSp();
    EvtSaveMapCheckpoint(0, 0);
    for (i = 0; i < 0x20; i++) {
        EvtSystemFlagSet(i + 0xA0, 0);
    }
    MapMiniAutoExploredClear();
}
