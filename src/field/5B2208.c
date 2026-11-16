#include "common.h"
#include "field/event.h"

void *FldMemAlloc(s32);
void MemSet(void *, s32, s32);

void EvtAreaInit(void) {
    evtAreaTable = FldMemAlloc(8);
    MemSet(evtAreaTable, 0, 8);
    evtAreaTable->count = 0;
}
