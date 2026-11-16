#include "common.h"
#include "protos/main.h"
#include "gph.h"
#include "main/n04.h"

void GphModelSkin(GphPart *, u16);
void GphModelMorph(GphPart *, u16, GphMotCursor *);
void GphModelCalcViewMatrices(GphModel *, s32);
void GphModelDrawPartsCulled(GphPart *, u16);
void GphModelDrawParts(GphPart *, u16);

void GphModelDrawMtx(GphModel *obj, s32 arg1) {
    s32 n;
    GphGaObject *e;
    GphModelMotion *mot;

    if (obj->flags & 8) {
        return;
    }
    *(GphModel **)0x70002800 = obj;
    *(s32 *)0x70002804 = 0;
    if (obj->cull < 0) {
        return;
    }
    n = obj->gaObjCount;
    e = obj->gaObjs;
    while (n > 0) {
        e->flags = 0;
        n--;
        e++;
    }
    if (obj->pktSize != 0) {
        if (GsPacketGetFree() < obj->pktSize) {
            return;
        }
        if (obj->unkB != 0) {
            GphModelSkin(obj->parts, obj->partCount);
        }
    }
    mot = (GphModelMotion *)&obj->motionMode;
    if (mot->slot[0].motion != NULL && (mot->slot[0].motion->flags & 0x30)) {
        GphMotCursorInit((GphMotCursor *)0x700028C0, mot->slot[0].motion, mot->slot[0].frame);
        GphModelMorph(obj->parts, obj->partCount, (GphMotCursor *)0x700028C0);
    }
    if (obj->cull == 0) {
        *(s32 *)0x70002804 = 1;
    }
    GphModelCalcViewMatrices(obj, arg1);
    if (obj->flags & 0x80) {
        GphModelDrawPartsCulled(obj->parts, obj->partCount);
    } else {
        GphModelDrawParts(obj->parts, obj->partCount);
    }
}
