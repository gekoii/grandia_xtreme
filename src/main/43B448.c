#include "common.h"
#include "gph.h"



GphPart *ChrModelFindPart(GphModel *arg0, s32 arg1) {
    s32 i;
    GphPart *p;

    p = arg0->parts;
    for (i = 0; i < arg0->partCount; i++, p++) {
        if (p->id == arg1) {
            return p;
        }
    }
    return NULL;
}
