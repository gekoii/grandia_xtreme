#include "common.h"
#include "protos/field.h"


s32 AmapRandBits(s16 lo, s16 hi) {
    s32 count;
    s32 i;
    u16 bits;
    s32 n;

    n = 0;
    bits = 0;
    count = VuRandInt(hi - lo + 1) + lo;
    do {
        for (i = 0; i < hi && n < count; i++) {
            if ((VuRandInt(0xFF) & 1) && !((bits >> i) & 1)) {
                bits |= 1 << i;
                n++;
            }
        }
    } while (n < count);
    return bits;
}
