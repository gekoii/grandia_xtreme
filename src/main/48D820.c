/* Sony PS2 SDK library code: see README.md, "Third-party library code". */
#include "common.h"

typedef union {
    double d;
    u64 l;
} DU;

s32 _scePrintfDoubleToInt(double x) {
    DU u;
    s64 e;
    u64 m;

    u.d = x;
    m = u.l;
    e = (m << 1) >> 53;
    e -= 0x433;
    if (e < -0x35) {
        return 0;
    }
    if (e >= 0xD) {
        return 9999;
    }
    m = (m << 12) >> 12;
    m = m | ((u64)0x8000 << 37);
    if (e < 0) {
        e = -e;
        m >>= (s32)(e - 2);
        if ((m & 3) == 3) {
            m = (m >> 2) + 1;
        } else {
            m >>= 2;
        }
    } else {
        m <<= (s32)e;
    }
    return m;
}
