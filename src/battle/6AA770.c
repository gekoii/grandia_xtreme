#include "common.h"
#include "battle/unit.h"
#include "gph.h"
#include "protos/battle.h"

void BtlChrSetMotion(BtlUnit *arg0, s32 arg1) {
    s32 idx;
    GphMotion *sub;
    s16 *t;

    idx = arg1;
    if (arg0->model != NULL) {
        if (arg0->flags & 0x200) {
            idx = BtlChrMotionAltIndex(arg1);
        }
        sub = arg0->def->motions[idx];
        if (sub != NULL) {
            t = arg0->def->motionEvents[idx];
            arg0->motionNo = arg1;
            arg0->motion = sub;
            arg0->evtLoop = t;
            arg0->evtStart = t;
            arg0->evtCur = t;
            arg0->unk128 = arg0->unk12C = 0.0f;
            if (btlActingUnit == 0xFF) {
                arg0->headLook = 0;
            }
            if (arg1 == 1 || arg1 == 2 || arg1 == 8) {
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
}
