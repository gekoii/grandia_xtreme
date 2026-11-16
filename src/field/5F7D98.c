#include "common.h"
#include "field/n20.h"
#include "protos/field.h"

FldMoveCmdPool *FldMemAlloc(s32);

void FldMoveCmdPoolInit(void) {
    FldMoveCmdPool **q = &fldMoveCmdPool;
    FldMoveCmdPool *p;

    p = FldMemAlloc(0x5010);
    *q = p;
    p->count = 0x100;
    FldMoveCmdPoolClear();
}
