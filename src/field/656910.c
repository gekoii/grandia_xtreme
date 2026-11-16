#include "common.h"
#include "field/n25.h"

extern void *FldMemAlloc(s32);
extern void MemSet(void *, s32, s32);

void FldTitleInit(void) {
    void **q;

    q = (void **)&fldTitleWork;
    *q = FldMemAlloc(0x7C0);
    MemSet(*q, 0, 0x7C0);
}
