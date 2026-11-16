#include "common.h"
#include "field/n20.h"

FldMoveCmd *FldMoveCmdAlloc(void) {
    s32 i;
    FldMoveCmd *p;

    for (i = 0; i < fldMoveCmdPool->count; i++) {
        if (fldMoveCmdPool->slots[i].used == 0) {
            fldMoveCmdPool->slots[i].used = 1;
            fldMoveCmdPool->slots[i].cmd.next = NULL;
            p = &fldMoveCmdPool->slots[i].cmd;
            p->id = fldMoveCmdNextId++;
            return p;
        }
    }
    return NULL;
}
