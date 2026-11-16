#include "common.h"
#include "field/n20.h"
#include "protos/field.h"

void FldObjMoveCmdRemove(ChrObj *o, FldMoveCmd *n) {
    FldMoveCmd *cur;
    FldMoveCmd *prev;

    if (o->moveCmds == n) {
        o->moveCmds = n->next;
        FldMoveCmdFree(n);
        return;
    }
    prev = NULL;
    cur = o->moveCmds;
    goto loop; /* goto loop with the found block first: the original's layout (a while loop differs) */
found:
    prev->next = cur->next;
    ((s32 (*)(FldMoveCmd *))FldMoveCmdFree)(cur); /* s32 cast: keeps the call out of tail position (jal) */
    return;
loop:
    if (cur == NULL) return;
    if (cur == n) goto found;
    prev = cur;
    cur = cur->next;
    goto loop;
}
