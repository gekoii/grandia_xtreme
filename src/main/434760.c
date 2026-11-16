#include "common.h"
#include "protos/main.h"
#include "gph.h"
#include "main/n04.h"

void GphModelCalcPartsRest(GphPart *, u16);
void GphModelCalcPartsMotion(GphPart *, u16, s32);
void GphModelCalcPartsMotionSplit(GphPart *, u16, s32, s32);
void GphModelCalcPartsMotionBlend(GphPart *, u16, s32, s32, f32);
void GphModelCalcPartsMotionOverride(GphPart *, u16, s32);
void GphModelCalcPartsMotionEx(GphPart *, u16, s32);

void GphModelCalcParts(GphModel *arg0) {
    GphModelMotion *m;
    GphPart *s1;
    u16 s2;

    m = (GphModelMotion *)&arg0->motionMode;
    s1 = arg0->parts;
    s2 = arg0->partCount;
    if (m->slot[0].motion == NULL) {
        GphModelCalcPartsRest(s1, s2);
        return;
    }
    switch (arg0->motionMode) {
    case 0:
        GphMotCursorInit(0x700028C0, m->slot[0].motion, m->slot[0].frame);
        if (!(m->slot[0].motion->flags & 0xFF0)) {
            GphModelCalcPartsMotion(s1, s2, 0x700028C0);
            return;
        }
        GphModelCalcPartsMotionEx(s1, s2, 0x700028C0);
        return;
    case 1:
        GphMotCursorInit(0x700028C0, m->slot[0].motion, m->slot[0].frame);
        GphMotCursorInit(0x700028E0, m->slot[1].motion, m->slot[1].frame);
        GphModelCalcPartsMotionSplit(s1, s2, 0x700028C0, 0x700028E0);
        return;
    case 2:
        GphMotCursorInit(0x700028C0, m->slot[0].motion, m->slot[0].frame);
        GphMotCursorInit(0x700028E0, m->slot[1].motion, m->slot[1].frame);
        GphModelCalcPartsMotionBlend(s1, s2, 0x700028C0, 0x700028E0, m->blend);
        return;
    case 3:
        GphMotCursorInit(0x700028C0, m->slot[0].motion, m->slot[0].frame);
        GphModelCalcPartsMotionOverride(s1, s2, 0x700028C0);
        return;
    default:
        return;
    }
}
