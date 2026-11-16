#include "common.h"
#include "protos/field.h"
#include "field/n20.h"


s16 FldMoveCmdGlobalAdd31(f32 *v, f32 f) {
    FldMoveCmd *n;

    n = FldMoveCmdAlloc();
    n->kind = 3;
    n->type = 1;
    n->state = 0;
    n->flags = 0;
    n->u.move.pos[0] = *v++;
    n->u.move.pos[1] = v[0];
    n->u.move.pos[2] = v[1];
    n->u.move.speed = f;
    FldMoveCmdGlobalAppend(n);
    return n->id;
}
