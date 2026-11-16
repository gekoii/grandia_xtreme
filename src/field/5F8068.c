#include "common.h"
#include "field/n20.h"

void FldMoveCmdGlobalAppend(FldMoveCmd *nd) {
    FldMoveCmd *p;
    FldMoveCmd *q;
    FldMoveCmd *t;

    p = fldMoveCmdGlobalList;
    if (p == NULL) {
        fldMoveCmdGlobalList = nd;
        return;
    }
    q = p->next;
    if (q != NULL) {
        do {
            p = q;
            t = p->next;
            q = t;
        } while (t != NULL);
    }
    p->next = nd;
    nd->next = NULL;
}
