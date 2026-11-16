#include "common.h"
#include "field/n20.h"

void FldObjMoveCmdAppend(ChrObj *o, FldMoveCmd *nd) {
    FldMoveCmd *p;
    FldMoveCmd *q;
    FldMoveCmd *t;

    if (o->moveCmds == 0) {
        o->moveCmds = nd;
        nd->next = NULL;
        return;
    }
    p = o->moveCmds;
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
