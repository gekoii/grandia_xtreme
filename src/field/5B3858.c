#include "common.h"
#include "field/event.h"

extern void EvtAreaDebugDraw(EvtArea *o);

void EvtAreaDebugDrawAll(void) {
    s32 i;

    for (i = 0; i < evtAreaTable->count; i++) {
        EvtAreaDebugDraw(&evtAreaTable->areas[i]);
    }
}
