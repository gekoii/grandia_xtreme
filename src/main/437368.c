#include "common.h"
#include "gph.h"

void ChrPartSetGaObjFlag2(GphPart *obj, s32 on) {
    s32 n = obj->gaObjCount;
    GphGaObject *e = obj->gaObjs;
    if (on) {
        for (; n > 0; n--) {
            e->mode |= 2;
            e++;
        }
    } else {
        for (; n > 0; n--) {
            e->mode &= ~2;
            e++;
        }
    }
}
