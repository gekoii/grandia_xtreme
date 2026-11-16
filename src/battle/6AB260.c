#include "common.h"
#include "battle/unit.h"

f32 BtlChrMotionFindKey(BtlUnit *arg0, s32 arg1, s16 **arg2) {
    s16 *p = arg0->evtCur;
    f32 v;

    while (p != NULL && *p >= 0) {
        v = *p++;
        if (*p++ == arg1) {
            if (arg2 != NULL) {
                *arg2 = p;
            }
            goto end; /* goto: an early return here moves the exit block */
        }
        p += *p + 1;
    }
    return arg0->frame;
end:
    return v;
}
