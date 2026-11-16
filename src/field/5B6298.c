#include "common.h"
#include "field/event.h"

void OccsStopLoop(s16 i) {
    u16 flags;
    OccsEntry *e;

    if (i >= 0 && i < occsTable->count) {
        e = (OccsEntry *)(i * 0x98 + (u32)occsTable->items);
        flags = e->flags;
        if (flags & OCC_ACTIVE) {
            e->flags = flags | OCC_STOP_LOOP;
        }
    }
}

void OccsSleep(s16 i) {
    u16 flags;
    OccsEntry *e;

    if (i >= 0 && i < occsTable->count) {
        e = (OccsEntry *)(i * 0x98 + (u32)occsTable->items);
        flags = e->flags;
        if (flags & OCC_ACTIVE) {
            e->state = OCC_SLEEP;
            e->flags = flags & 0xFFEF;
        }
    }
}
