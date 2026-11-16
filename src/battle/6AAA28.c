#include "common.h"
#include "battle/unit.h"
#include "gph.h"

void BtlChrSetMotionData(BtlUnit *arg0, GphMotion *arg1, s16 *arg2) {
    if (arg0->model != NULL && arg1 != NULL) {
        arg0->motionNo = -1;
        arg0->motion = arg1;
        arg0->evtLoop = arg2;
        arg0->evtStart = arg2;
        arg0->evtCur = arg2;
        arg0->unk128 = arg0->unk12C = 0.0f;
        if (btlActingUnit == 0xFF) {
            arg0->headLook = 0;
        }
        arg0->headLook |= 1;
        arg0->frame = arg0->loopStart = 0.0f;
        arg0->loopEnd = arg0->motionLast = arg0->motion->frameCount - 1.0f;
        arg0->loopCount = 0;
        arg0->evtIds[0] = 0;
        arg0->flags &= ~0x100u;
    }
}
