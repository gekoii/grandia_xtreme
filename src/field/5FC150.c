#include "common.h"

void EvtMsgGlyphMetrics(u16 *arg0, s32 *arg1, s32 *arg2) {
    /* identical arms (the original keeps the test and its load of *arg0) */
    if (*arg0 < 0x100) {
        *arg1 = 0;
        *arg2 = 0x18;
    } else {
        *arg1 = 0;
        *arg2 = 0x18;
    }
}
