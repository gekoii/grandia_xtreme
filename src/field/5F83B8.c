#include "common.h"
#include "protos/field.h"
#include "field/n20.h"


s16 FldMoveCmdGlobalAdd23(void) {
    FldMoveCmd *n;

    n = FldMoveCmdAlloc();
    n->state = 0;
    n->flags = 0;
    n->unk6 = 0;
    n->kind = 2;
    n->type = 3;
    FldMoveCmdGlobalAppend(n);
    return n->id;
}
