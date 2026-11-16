#include "common.h"

extern s32 fldIntDigitCount;

s32 FldIntToDigits(s32 n, u8 *buf) {
    s32 d;
    s32 div;
    s32 started = 0;

    fldIntDigitCount = 0;
    for (div = 10000000; div != 0; div /= 10) {
        d = n / div;
        if (d != 0 || started) {
            started = 1;
            buf[fldIntDigitCount++] = d < 0 ? -d : d;
        }
        n -= d * div;
    }
    if (fldIntDigitCount == 0) {
        buf[0] = 0;
        fldIntDigitCount = 1;
    }
    buf[fldIntDigitCount] = 0;
    return fldIntDigitCount;
}
