#include "common.h"
#include "protos/battle.h"
#include "gph.h"


GphPart *BtlChrFindPart(GphModel *p, s32 arg1) {
    GphPart *r;

    if (p == NULL) {
        return NULL;
    }
    r = ChrModelFindPart(p, arg1);
    if (r == NULL) {
        r = p->parts;
    }
    return r;
}
