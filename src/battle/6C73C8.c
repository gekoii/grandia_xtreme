#include "common.h"

u16 BtlAngleApproach(s32 cur, s32 tgt, s32 step) {
    s32 d;

    if (cur != tgt) {
        tgt &= 0xFFFF;
        cur &= 0xFFFF;
        d = tgt - cur;
        if (d > 0x8000) {
            tgt += -0x10000;
        } else if (d < -0x8000) {
            tgt += 0x10000;
        }
        if (tgt < cur) {
            cur -= step;
            if (cur < tgt) cur = tgt;
        } else if (cur < tgt) {
            cur += step;
            if (tgt < cur) cur = tgt;
        }
    }
    return cur;
}
