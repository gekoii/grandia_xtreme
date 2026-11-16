#include "common.h"
s32 VuIsqrt(s32 n) {
    s32 x;
    s32 y;
    y = 1;
    if (n == 0) {
        return 0;
    }
    x = n;
    while (y < x) {
        y *= 2;
        x >>= 1;
    }
    do {
        x = y;
        y = (n / x + x) >> 1;
    } while (y < x);
    return x;
}
