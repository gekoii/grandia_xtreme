/* Third-party library code (newlib, fdlibm-derived): see README.md, "Third-party library code". */
#include "common.h"
#include "protos/main.h"

typedef union {
    double value;
    struct {
        u32 lsw;
        u32 msw;
    } parts;
} ieee_double_shape_type;

double func_004812A8(double, double);

double cos(double x) {
    double y[2];
    s32 n;
    s32 ix;

    do {
        ieee_double_shape_type gh_u;
        gh_u.value = x;
        ix = gh_u.parts.msw;
    } while (0);
    ix &= 0x7FFFFFFF;
    if (ix <= 0x3FE921FB) {
        return __kernel_cos(x, 0.0);
    } else if (ix >= 0x7FF00000) {
        return func_004812A8(x, x);
    } else {
        n = __ieee754_rem_pio2(x, y);
        switch (n & 3) {
        case 0:
            return __kernel_cos(y[0], y[1]);
        case 1:
            return func_004812A8(0.0, __kernel_sin(y[0], y[1], 1));
        case 2:
            return func_004812A8(0.0, __kernel_cos(y[0], y[1]));
        default:
            return __kernel_sin(y[0], y[1], 1);
        }
    }
}
