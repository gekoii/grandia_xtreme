#include "common.h"
#include "protos/field.h"
#include "field/n20.h"


s16 FldMoveCmdGlobalAdd12(s32 arg0, f32 f) {
    FldMoveCmd *n;

    n = FldMoveCmdAlloc();
    n->u.f.b = f;
    n->u.f.c = arg0;
    n->state = 0;
    n->flags = 0;
    n->u.f.d = 0;
    n->kind = 1;
    n->type = 2;
    FldMoveCmdGlobalAppend(n);
    return n->id;
}
