#include "common.h"
#include "gph.h"

extern void VuVecCopy(void *, s32);

void ChrModelSetColor(GphModel *arg0, s32 arg1) {
    s32 i;
    GphPart *e;

    e = arg0->parts;
    for (i = 0; i < arg0->partCount; i++) {
        VuVecCopy(e->color, arg1);
        e++;
    }
}
