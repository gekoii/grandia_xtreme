/* as: ee-gcc2.9-991111-01 */
#include "common.h"
#include "battle/unit.h"

void BtlChrMotionSetFrame(BtlUnit *arg0, f32 arg1) {
    s16 *p;

    if (arg1 < 0.0f) {
        arg1 = 0.0f;
    } else if (arg0->motionLast < arg1) {
        arg1 = arg0->motionLast;
    }
    arg0->frame = arg1;
    p = arg0->evtCur;
    while (p != NULL) {
        if (*p < 0) {
            p = NULL;
            arg0->evtCur = NULL;
        } else if (*p < arg0->frame) {
            p += 2;
            p += *p + 1;
            arg0->evtCur = p;
        } else {
            p = NULL;
        }
    }
    arg0->evtIds[0] = 0;
    arg0->unk12C = 40000.0f;
    arg0->flags &= ~0x100u;
}
