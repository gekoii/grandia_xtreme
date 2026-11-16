#include "common.h"
#include "protos/field.h"


void FldMenuListCursorUp(u32 *cur, u32 *top, u32 rows, u32 lim, u32 count) {
    if (*top != 0) {
        if (*cur > lim) {
            (*cur)--;
            SndSePlay(0x7D1, 0, 0, 0x1E, 0);
        } else {
            (*top)--;
            SndSePlay(0x7D1, 0, 0, 0x1E, 0);
        }
    } else if (*cur != 0) {
        (*cur)--;
        /* cast: blocks the tail call, the original calls with jal and returns */
        ((s64 (*)(s32, s32, s32, s32, s32))SndSePlay)(0x7D1, 0, 0, 0x1E, 0);
    }
}
