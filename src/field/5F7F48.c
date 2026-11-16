#include "common.h"
#include "field/n20.h"

void FldMoveCmdFree(void *p) {
    s32 i;

    for (i = 0; i < fldMoveCmdPool->count; i++) {
        if (&fldMoveCmdPool->slots[i].cmd == p) {
            fldMoveCmdPool->slots[i].used = 0;
        }
    }
}
