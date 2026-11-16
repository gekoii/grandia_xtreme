#include "common.h"
#include "gph.h"

void GphGaObjDeformCollapse(GphGaObject *, f32, f32, f32);

void GphPartDeformCollapse(GphPart *arg0, f32 a, f32 b, f32 c) {
    s32 i;
    GphGaObject *e;

    i = arg0->gaObjCount;
    e = arg0->gaObjs;
    for (; i > 0; i--, e++) {
        if (e->deformBuf0 != NULL && !(e->clip & 0x80)) {
            GphGaObjDeformCollapse(e, a, b, c);
        }
    }
}
