#include "common.h"
#include "gph.h"

/* One parameter: 0x00437928 also passes the motion, through a cast; the other callers pass the model. */
void ChrModelClearDeformBufs(GphModel *arg0) {
    s32 i;
    s32 j;
    GphPart *g;
    GphGaObject *it;

    g = arg0->parts;
    i = arg0->partCount;
    do {
        it = g->gaObjs;
        for (j = g->gaObjCount; j > 0; j--) {
            it->deformBuf0 = NULL;
            it->deformBuf1 = NULL;
            it++;
        }
        g++;
    } while (--i > 0);
}
