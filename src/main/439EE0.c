#include "common.h"
#include "gph.h"

void GphModelCalcViewMatrices(GphModel *, s32);
void GphModelDrawSimpleParts(GphPart *, s32);

void GphModelDrawSimpleMtx(GphModel *obj, s32 arg1) {
    s8 v;

    if (obj->flags & 8) {
        return;
    }
    *(GphModel **)0x70002800 = obj;
    *(s32 *)0x70002804 = 0;
    v = obj->cull;
    if (v < 0) {
        return;
    }
    if (v == 0) {
        *(s32 *)0x70002804 = 1;
    }
    GphModelCalcViewMatrices(obj, arg1);
    /* identical arms: the original tests the flag and keeps both calls */
    if (obj->flags & 0x80) {
        GphModelDrawSimpleParts(obj->parts, obj->partCount);
    } else {
        GphModelDrawSimpleParts(obj->parts, obj->partCount);
    }
}
