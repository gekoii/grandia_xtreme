#include "common.h"
#include "protos/field.h"
#include "field/n20.h"


s16 FldMoveCmdGlobalAdd24(void) {
    FldMoveCmd *n;

    n = FldMoveCmdAlloc();
    n->state = 0;
    n->flags = 0;
    n->kind = 2;
    n->type = 4;
    FldMoveCmdGlobalAppend(n);
    return n->id;
}
