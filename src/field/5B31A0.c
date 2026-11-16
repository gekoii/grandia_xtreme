#include "common.h"
#include "field/event.h"

EvtArea *EvtAreaFindById(s32 id) {
    s32 n;
    s32 i;
    EvtArea *e;

    n = evtAreaTable->count;
    for (i = 0; i < n; i++) {
        e = &evtAreaTable->areas[i];
        if (e->id == id) return e;
    }
    return NULL;
}
