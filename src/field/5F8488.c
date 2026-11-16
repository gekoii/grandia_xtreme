/* as: ee-gcc2.9-991111-01 */
#include "common.h"
#include "protos/field.h"
#include "field/n20.h"


s16 FldMoveCmdGlobalAdd32(f32 *v, f32 f1, f32 f2) {
    FldMoveCmd *n;
    FldMoveCmdMove *p;

    if (v[0] == 0.0f && v[1] == 0.0f && v[2] == 0.0f) {
        FldMoveCmdGlobalCancelKind(3, 2);
        return 0;
    }
    n = FldMoveCmdAlloc();
    n->kind = 3;
    n->type = 2;
    n->state = 0;
    n->flags = 0;
    n->unk6 = 0;
    p = &n->u.move;
    n->u.move.pos[0] = *v++;
    n->u.move.pos[1] = v[0];
    n->u.move.pos[2] = v[1];
    p->speed = f1;
    p->speed2 = f2;
    FldMoveCmdGlobalAppend(n);
    return n->id;
}
