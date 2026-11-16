#include "common.h"
#include "field/event.h"

extern void *FldMemAlloc(s32 size);
extern void MemSet(void *, s32, s32);

s32 OccInit(s32 arg0) {
    occTable = FldMemAlloc(8);
    MemSet(occTable, 0, 8);
    occTable->count = 0;
    return arg0;
}
