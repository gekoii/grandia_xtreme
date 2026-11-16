/* cflags: -fno-gcse */
/* -fno-gcse: without it gcse moves the node loads out of the search loop (one insn fewer, other registers) */
#include "common.h"
#include "field/n20.h"
#include "protos/field.h"

s32 FldObjMoveCmdRemoveById(ChrObj *o, s16 id) {
    FldMoveCmd *p;
    FldMoveCmd *q;
    FldMoveCmd *r;

    p = o->moveCmds;
    if (p == NULL) {
        return 0;
    }
    if (p->id == id) {
        *(s32 *)&o->moveCmds = (s32)p->next; /* an s32 store: as a pointer store it aliases p->next, which is then reloaded */
        p = p->next;
    }
    q = p->next;
    if (q != NULL) {
        r = q;
loop: /* goto loop: never advances r, so a miss spins forever (as in the original); a while loop differs */
        if (r->id != id) goto loop;
        p->next = q->next;
        FldMoveCmdFree(q);
        return 1;
    }
    return 0;
}
