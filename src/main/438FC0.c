#include "common.h"
#include "gph.h"

extern void GphGaObjDeformWaveLit(GphGaObject *, f32, f32, f32);

void GphModelDeformWaveLit(GphModel *arg0, f32 a, f32 b, f32 c) {
    s32 n;
    GphGaObject *p;

    n = arg0->gaObjCount;
    p = arg0->gaObjs;
    for (; n > 0; n--, p++) {
        if (p->vtxPkt != NULL && !(p->clip & 0x80)) {
            GphGaObjDeformWaveLit(p, a, b, c);
        }
    }
}
