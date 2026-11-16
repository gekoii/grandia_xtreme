#include "common.h"
#include "protos/main.h"
#include "gph.h"


void GphModelDeformVortex(GphModel *arg0, f32 a, f32 b, f32 c) {
    s32 n;
    GphGaObject *p;

    n = arg0->gaObjCount;
    p = arg0->gaObjs;
    while (n > 0) {
        if (!(p->clip & 0x80)) {
            GphGaObjDeformVortex(p, a, b, c);
        }
        n--;
        p++;
    }
}
