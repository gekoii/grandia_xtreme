#include "common.h"
#include "field/n20.h"

s32 FldMoveCmdGlobalCancel(s16 arg0) {
    FldMoveCmd *p;
    FldMoveCmd *n;
    FldMoveCmd *t;

    p = fldMoveCmdGlobalList;
    if (p == NULL) {
        return 0;
    }
    if (p->id == arg0) {
        p->state = 2;
    }
    n = p->next;
    if (n != NULL) {
        do {
            if (n->id == arg0) { n->state = 2; return 1; }
            t = n->next;
            n = t;
        } while (t != NULL);
    }
    return 0;
}
