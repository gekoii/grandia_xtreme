#include "common.h"
#include "gph.h"

s32 ChrPartCalcDeformBufSize(GphPart *arg0) {
    s32 i;
    s32 total;
    GphGaObject *it;

    if (arg0 == NULL) {
        return 0;
    }
    i = arg0->gaObjCount;
    total = 0;
    it = arg0->gaObjs;
    for (; i > 0; i--) {
        if (it->skin == NULL) {
            total += (it->vtxCount * 16 + 0x3F) & 0xFFFFFFC0;
            total += (it->vtxCount * 8 + 0x4F) & 0xFFFFFFC0;
        }
        it++;
    }
    return total;
}
