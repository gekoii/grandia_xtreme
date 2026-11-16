#include "common.h"
#include "protos/battle.h"


s32 BtlMenuCursorStep(s32 pad, s32 cur, s32 min, s32 max) {
    u8 wrap;
    wrap = max - min >= 2;

    if (pad & 0x9005) {
        SndSePlay3D(0x7D1, 0, 0, 0);
        if (cur <= min) {
            return wrap ? max : min;
        }
        return cur - 1;
    }
    if (pad & 0x600A) {
        SndSePlay3D(0x7D1, 0, 0, 0);
        if (max <= cur) {
            return wrap ? min : max;
        }
        return cur + 1;
    }
    return cur;
}
