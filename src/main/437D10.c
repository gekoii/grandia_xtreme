#include "common.h"
#include "gph.h"

s32 ChrModelCalcWorkSize(GphModel *arg0) {
    s32 n;
    s32 total;
    s32 sum;
    s32 j;
    GphPart *sub;
    GphGaObject *e;
    s32 i;

    n = arg0->partCount;
    sub = arg0->parts;
    total = n * 0xA0 + 0xC0;
    total += (n + 1) * 64;
    j = n;
    do {
        e = sub->gaObjs;
        sum = 0;
        for (i = sub->gaObjCount; i > 0; i--) {
            sum += 0x80;
            sum += (e->vtxCount * 16 + 0x3F) & 0xFFFFFFC0;
            sum += (e->vtxCount * 8 + 0x4F) & 0xFFFFFFC0;
            sum += (e->vtxCount * 4 + 0x4F) & 0xFFFFFFC0;
            sum += (e->vtxCount * 4 + 0x4F) & 0xFFFFFFC0;
            sum += (e->triCount * 8 + 0x3F) & 0xFFFFFFC0;
            e++;
        }
        total += sum;
    } while (--j > 0);
    return total;
}
