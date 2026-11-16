#include "common.h"
#include "field/event.h"


EvtArea *EvtAreaGet(s32 i) {
    if (i < 0) return NULL;
    if (i >= evtAreaTable->count) return NULL;
    return &evtAreaTable->areas[i];
}
