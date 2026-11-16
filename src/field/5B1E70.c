#include "common.h"

extern s32 fldUIntDigitCount;

s32 FldUIntToDigits(u32 n, u8 *buf, u32 div) {
    u32 d;
    s32 started = 0;

    div *= 10;
    fldUIntDigitCount = 0;
    while (div != 0) {
        d = n / div;
        if (d != 0 || started) {
            started = 1;
            buf[fldUIntDigitCount++] = d;
        }
        n -= d * div;
        div /= 10;
    }
    if (fldUIntDigitCount == 0) {
        buf[0] = 0;
        fldUIntDigitCount = 1;
    }
    buf[fldUIntDigitCount] = 0;
    return fldUIntDigitCount;
}
