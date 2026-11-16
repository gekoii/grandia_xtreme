#include "common.h"
#include "field/event.h"

void OccWake(s16 i) {
    OccEntry *e;

    if (i >= 0 && i < occTable->count) {
        e = (OccEntry *)((i << 7) + (u32)occTable->items);
        if (e->state == OCC_SLEEP) {
            e->state = OCC_WAKE;
            e->flags |= OCC_ACTIVE;
        }
    }
}

void OccStopLoop(s16 i) {
    u16 flags;
    OccEntry *e;

    if (i >= 0 && i < occTable->count) {
        e = (OccEntry *)((i << 7) + (u32)occTable->items);
        flags = e->flags;
        if (flags & OCC_ACTIVE) {
            e->flags = flags | OCC_STOP_LOOP;
        }
    }
}

void OccSleep(s16 i) {
    u16 flags;
    OccEntry *e;

    if (i >= 0 && i < occTable->count) {
        e = (OccEntry *)((i << 7) + (u32)occTable->items);
        flags = e->flags;
        if (flags & OCC_ACTIVE) {
            e->state = OCC_SLEEP;
            e->flags = flags & 0xFFEF;
        }
    }
}

s32 OccIsSleeping(s16 i) {
    if (i < occTable->count) {
        return occTable->items[i].state == OCC_SLEEP;
    }
    return -1;
}
