#include "common.h"

s8 BtlResultCountDigitFrames(s32 a, s32 b, s32 unit) {
    s32 big;
    s8 r;

    big = unit * 10;
    a = a % big - a % unit;
    b = b % big - b % unit;
    r = (b - a) / unit;
    if (r <= 0) {
        r += 10;
    }
    return r + 30;
}
