#include "common.h"
#include "field/n14.h"

void *FldMemAlloc(s32);
void MemSet(void *, s32, s32);

void FldDmgPopInit(void) {
    FldDmgPopWork **p = &fldDmgPopWork;
    void *v;
    v = FldMemAlloc(0x4450);
    *p = v;
    MemSet(v, 0, 0x4450);
}
