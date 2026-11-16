#include "common.h"
#include "protos/main.h"
#include "gph.h"

void GphModelCalcViewMatrices(GphModel *, s32);
void GphModelDrawCopyParts(GphPart *, s32);

void GphModelDrawCopyMtx(GphModel *obj, s32 arg1) {
    s8 v;

    if (obj->flags & 8) {
        return;
    }
    if (obj->pktSize != 0 && GsPacketGetFree() < obj->pktSize) {
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
        GphModelDrawCopyParts(obj->parts, obj->partCount);
    } else {
        GphModelDrawCopyParts(obj->parts, obj->partCount);
    }
}
