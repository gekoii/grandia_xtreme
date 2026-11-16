#include "common.h"
#include "gph.h"

s32 ChrModelCalcVtxBufSize(GphModel *arg0, s32 arg1) {
    s32 n;
    s32 sum;
    GphGaObject *p;

    if (arg0 == NULL) {
        return 0;
    }
    n = arg0->gaObjCount;
    sum = 0;
    p = arg0->gaObjs;
    for (; n > 0; n--, p++) {
        if (arg1 & 1) {
            sum += (p->vtxCount * 16 + 0x3F) & 0xFFFFFFC0;
        }
        if (arg1 & 2) {
            sum += (p->vtxCount * 8 + 0x4F) & 0xFFFFFFC0;
        }
        if (arg1 & 4) {
            sum += (p->vtxCount * 4 + 0x4F) & 0xFFFFFFC0;
        }
        if (arg1 & 8) {
            sum += (p->vtxCount * 4 + 0x4F) & 0xFFFFFFC0;
        }
    }
    return sum;
}
