#include "common.h"
#include "field/n25.h"

extern void *FldMemAlloc(s32);
extern void MemSet(void *, s32, s32);

void FldFlagListInit(void) {
    void **q;

    q = (void **)&fldFlagListWork;
    *q = FldMemAlloc(0x6460);
    MemSet(*q, 0, 0x6460);
}
