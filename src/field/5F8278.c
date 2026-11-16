#include "common.h"
#include "protos/field.h"
#include "field/n20.h"


s16 FldMoveCmdGlobalAdd11(s32 arg0, s32 arg1) {
    FldMoveCmd *n;

    n = FldMoveCmdAlloc();
    n->u.i.a = arg0;
    n->u.i.c = arg1;
    n->state = 0;
    n->flags = 0;
    n->u.i.d = 0;
    n->kind = 1;
    n->type = 1;
    FldMoveCmdGlobalAppend(n);
    return n->id;
}
