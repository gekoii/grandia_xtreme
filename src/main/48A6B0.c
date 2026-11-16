/* Third-party library code (newlib): see README.md, "Third-party library code". */
#include "common.h"

typedef union {
    double d;
    u32 L[2];
} U;

double ulp(double x) {
    s32 L;
    U a;
    U ux;

    ux.d = x;
    L = (ux.L[1] & 0x7FF00000) - 0x3400000;
    if (L > 0) {
        a.L[1] = L;
        a.L[0] = 0;
    } else {
        L = -L >> 20;
        if (L < 20) {
            a.L[1] = 0x80000 >> L;
            a.L[0] = 0;
        } else {
            a.L[1] = 0;
            L -= 20;
            a.L[0] = L >= 31 ? 1 : 1 << (31 - L);
        }
    }
    return a.d;
}
