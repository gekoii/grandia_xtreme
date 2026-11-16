#include "common.h"
#include "protos/field.h"
#include "field/n20.h"


s16 FldObjMoveVelocity(s32 arg0, f32 *v, s32 fl, f32 f) {
    FldMoveCmd *n;

    n = FldMoveCmdAlloc();
    n->state = 0;
    n->type = 3;
    n->flags = 0;
    n->kind = 0;
    n->u.move.pos[0] = v[0];
    n->u.move.pos[1] = v[1];
    n->u.move.pos[2] = v[2];
    n->u.move.speed = f;
    if (fl & 1) {
        n->flags = 0x10;
    }
    if (fl & 2) {
        n->flags |= 0x20;
    }
    FldObjMoveCmdAppend(arg0, n);
    return n->id;
}
