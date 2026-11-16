#include "common.h"
#include "field/n25.h"

extern void *FldMemAlloc(s32);
extern void MemSet(void *, s32, s32);

void FldPictureInit(void) {
    void **q;

    q = (void **)&fldPictureWork;
    *q = FldMemAlloc(0x100);
    MemSet(*q, 0, 0x100);
}
