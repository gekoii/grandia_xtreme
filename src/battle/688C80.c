#include "common.h"
#include "protos/battle.h"

void BtlResultCountStep(s32 *cur, s32 max, s32 *unit, s8 *cnt) {
    *cur += *unit;
    if (*cur >= max) {
        *cur = max;
        return;
    }
    if (--*cnt <= 0) {
        *unit *= 10;
        *cnt = BtlResultCountDigitFrames(*cur, max, *unit);
    }
}
