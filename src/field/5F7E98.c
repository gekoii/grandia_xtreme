#include "common.h"
#include "field/n20.h"

s32 FldMoveCmdGlobalCancelKind(s32 arg0, s32 arg1) {
    FldMoveCmd *n;

    for (n = fldMoveCmdGlobalList; n != NULL; n = n->next) {
        if (n->kind == arg0 && n->type == arg1) {
            n->state = 2;
        }
    }
    return 1;
}
