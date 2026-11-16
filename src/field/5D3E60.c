#include "common.h"
#include "field/state.h"

void *FldMemAlloc(s32);
void MemSet(void *, s32, s32);

void FldTreasureInit(void) {
    FldTreasure **p = &fldTreasureWork;
    void *t = FldMemAlloc(0x2410);
    *p = t;
    MemSet(t, 0, 0x2410);
}
