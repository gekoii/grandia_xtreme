#include "common.h"
#include "protos/field.h"
#include "field/n20.h"


s16 FldMoveCmdGlobalAdd42(s32 a, s32 b, s32 e, f32 c, f32 d) {
    FldMoveCmd *n;
    FldMoveCmdK4 *p;

    n = FldMoveCmdAlloc();
    n->kind = 4;
    n->type = 2;
    n->state = 0;
    n->flags = 0;
    p = &n->u.k4;
    p->a = a;
    p->c = c;
    p->d = d;
    p->b = b;
    p->e = e;
    FldMoveCmdGlobalAppend(n);
    return n->id;
}
