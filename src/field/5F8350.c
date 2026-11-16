#include "common.h"
#include "protos/field.h"
#include "field/n20.h"


s16 FldMoveCmdGlobalAdd22(s32 arg0, s32 arg1) {
    FldMoveCmd *n;

    n = FldMoveCmdAlloc();
    n->u.i.a = arg1;
    n->u.i.b = arg0;
    n->state = 0;
    n->flags = 0;
    n->kind = 2;
    n->type = 2;
    FldMoveCmdGlobalAppend(n);
    return n->id;
}
