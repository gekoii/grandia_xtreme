#include "common.h"

extern s32 fldIntDigitCountN;

#define ABS(x) ((x) < 0 ? -(x) : (x))

s32 FldIntToDigitsN(s32 n, u8 *buf, s32 cnt) {
    s32 v;
    s32 j;

    fldIntDigitCountN = 0;
    while (cnt != 0) {
        cnt--;
        v = n;
        for (j = 0; j < cnt; j++) {
            v /= 10;
        }
        if (v != 0) {
            buf[fldIntDigitCountN++] = ABS(v % 10);
        }
    }
    if (fldIntDigitCountN == 0) {
        buf[0] = 0;
        fldIntDigitCountN = 1;
    }
    buf[fldIntDigitCountN] = 0;
    return fldIntDigitCountN;
}
