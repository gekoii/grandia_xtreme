#include "common.h"
#include "field/event.h"

extern void *FldMemAlloc(s32 size);
extern void MemSet(void *, s32, s32);

s32 OccsInit(s32 arg0) {
    occsTable = FldMemAlloc(8);
    MemSet(occsTable, 0, 8);
    occsTable->count = 0;
    return 0;
}
