#include "common.h"
#include "field/n20.h"

void FldMoveCmdPoolClear(void) {
    s32 i;

    for (i = 0; i < fldMoveCmdPool->count; i++) {
        fldMoveCmdPool->slots[i].used = 0;
    }
    fldMoveCmdGlobalList = NULL;
    fldMoveCmdNextId = 0;
}
