#include "common.h"
#include "protos/field.h"


void FldMenuListCursorDown(u32 *cur, u32 *top, u32 rows, u32 lim, u32 total) {
    if (total < rows) {
        if (*cur < total - 1) {
            (*cur)++;
            SndSePlay(0x7D1, 0, 0, 0x1E, 0);
        }
    } else if (*cur < lim) {
        (*cur)++;
        SndSePlay(0x7D1, 0, 0, 0x1E, 0);
    } else if (*top + rows < total) {
        (*top)++;
        SndSePlay(0x7D1, 0, 0, 0x1E, 0);
    } else if (*cur < rows - 1) {
        (*cur)++;
        /* cast: blocks the tail call, the original calls with jal and returns */
        ((s64 (*)(s32, s32, s32, s32, s32))SndSePlay)(0x7D1, 0, 0, 0x1E, 0);
    }
}
