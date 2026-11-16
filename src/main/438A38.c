#include "common.h"
#include "gph.h"

void GphGaObjDeformPulse(GphGaObject *, f32, f32, f32);

void GphModelDeformPulse(GphModel *arg0, f32 a, f32 b, f32 c) {
    s32 i;
    GphGaObject *e;

    e = arg0->gaObjs;
    for (i = 0; i < arg0->gaObjCount; i++, e++) {
        if (e->deformBuf0 != NULL && !(e->clip & 0x80)) {
            GphGaObjDeformPulse(e, a, b, c);
        }
    }
}
