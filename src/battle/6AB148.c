#include "common.h"
#include "battle/unit.h"

s32 BtlChrMotionGetEvent(BtlUnit *arg0, s32 arg1, s16 **arg2) {
    s32 *a = arg0->evtIds;
    s16 **b = arg0->evtArgs;

    while (*a != 0) {
        if (*a == arg1) {
            if (arg2 != NULL) {
                *arg2 = *b;
            }
            return 1;
        }
        a++;
        b++;
    }
    return 0;
}
