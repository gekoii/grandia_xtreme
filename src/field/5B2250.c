#include "common.h"
#include "field/event.h"
#include "field/n12.h"

void *FldMemAlloc(s32);

void EvtAreaAlloc(s32 n) {
    EvtArea *p;

    evtAreaTable->count = n;
    if (n != 0) {
        p = FldMemAlloc(n << 8);
        if (p != NULL) {
            evtAreaTable->areas = p;
        } else {
            evtAreaTable->count = 0;
            evtAreaTable->areas = NULL;
        }
        evtAreaUse = 1;
        evtAreaFrameWorkA = 0;
        evtAreaFrameWorkB = 0;
        evtAreaStepPriority = 0;
        evtAreaStepSound = 0;
    }
}
