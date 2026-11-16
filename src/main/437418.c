#include "common.h"
#include "gph.h"

void ChrPartSetGaObjMode(GphPart *o, f32 v) {
    s32 n;
    GphGaObject *p;

    n = o->gaObjCount;
    p = o->gaObjs;
    while (n > 0) {
        p->mode = v;
        n--;
        p++;
    }
}
