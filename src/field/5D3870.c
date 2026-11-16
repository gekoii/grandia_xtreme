#include "common.h"
#include "save.h"
#include "protos/field.h"


void MemSet(void *, s32, s32);

void EvtMapStateReset(void) {
    s32 i;
    u8 *p;

    EvtSystemFlagSet(0x84, 0);
    EvtSystemFlagSet(0xFA, 0);
    EvtSystemFlagSet(0x65, 0);
    p = evtLocalFlags;
    MemSet(p, 0, 0x10);
    MemSet(p - 0x282, 0, 0x20);
    MemSet(p - 0x262, 0, 8);
    MemSet(p - 0x25A, 0, 4);
    for (i = 0; i < 0x20; i++) {
        EvtSystemFlagSet(i + 0xA0, 0);
    }
    MapMiniAutoExploredClear();
}
