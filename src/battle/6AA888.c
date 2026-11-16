#include "common.h"
#include "battle/unit.h"
#include "gph.h"
#include "protos/battle.h"

void BtlChrSetMotionOf(BtlUnit *arg0, BtlUnit *arg1, s32 arg2) {
    s32 idx;
    BtlCharDef *tbl;
    GphMotion *sub;
    GphMotion *s2;

    idx = arg2;
    if (arg0->model != NULL) {
        if (arg0->flags & 0x200) {
            idx = BtlChrMotionAltIndex(arg2);
        }
        tbl = arg1->def;
        sub = tbl->motions[idx];
        if (sub == tbl->motions[0]) {
            BtlCharDef *e;
            s16 *t;
            s2 = arg0->def->motions[idx];
            if (s2 == NULL) return;
            e = (BtlCharDef *)((s32 *)arg0->def + idx);
            arg0->motion = s2;
            t = e->motionEvents[0];
            arg0->evtLoop = t;
            arg0->evtStart = t;
            arg0->evtCur = t;
        } else {
            BtlCharDef *e;
            s16 *t;
            e = (BtlCharDef *)((s32 *)tbl + idx);
            arg0->motion = sub;
            t = e->motionEvents[0];
            arg0->evtLoop = t;
            arg0->evtStart = t;
            arg0->evtCur = t;
        }
        arg0->motionNo = arg2;
        arg0->unk128 = arg0->unk12C = 0.0f;
        if (btlActingUnit == 0xFF) {
            arg0->headLook = 0;
        }
        if (arg2 == 1 || arg2 == 2 || arg2 == 8) {
            arg0->headLook &= ~1;
        } else {
            arg0->headLook |= 1;
        }
        arg0->frame = arg0->loopStart = 0.0f;
        arg0->loopEnd = arg0->motionLast = arg0->motion->frameCount - 1.0f;
        arg0->loopCount = 0;
        arg0->evtIds[0] = 0;
        arg0->flags &= ~0x100u;
    }
}
