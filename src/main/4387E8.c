#include "common.h"
#include "protos/main.h"
#include "gph.h"


void GphModelDeformSway(GphModel *arg0, f32 a, f32 b, f32 c) {
    s32 i;
    GphGaObject *p;

    p = arg0->gaObjs;
    for (i = 0; i < arg0->gaObjCount; i++, p++) {
        if (p->deformBuf0 != NULL && !(p->clip & 0x80)) {
            GphGaObjDeformSway(p, a, b, c);
        }
    }
}
