#include "common.h"
#include "battle/unit.h"

void BtlChrMotionRestart(BtlUnit *arg0) {
    arg0->flags &= ~0x100u;
    arg0->frame = arg0->loopStart = 0.0f;
    arg0->loopCount = 0;
    arg0->evtLoop = arg0->evtStart;
    arg0->evtCur = arg0->evtStart;
    arg0->evtIds[0] = 0;
    arg0->loopEnd = arg0->motionLast;
}
