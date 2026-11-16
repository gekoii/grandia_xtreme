/* Third-party library code (newlib, fdlibm-derived): see README.md, "Third-party library code". */
#include "common.h"
#include "protos/main.h"

typedef union {
    f32 value;
    u32 word;
} ieee_float_shape_type;

#define GET_FLOAT_WORD(i, d)            \
    do {                                \
        ieee_float_shape_type gf_u;     \
        gf_u.value = (d);               \
        (i) = gf_u.word;                \
    } while (0)


f32 tanf(f32 x) {
    f32 y[2];
    f32 z = 0.0f;
    s32 n;
    s32 ix;

    GET_FLOAT_WORD(ix, x);
    ix &= 0x7fffffff;
    if (ix <= 0x3f490fda) {
        return __kernel_tanf(x, z, 1);
    } else if (ix >= 0x7f800000) {
        return x - x;
    } else {
        n = __ieee754_rem_pio2f(x, y);
        return __kernel_tanf(y[0], y[1], 1 - ((n & 1) << 1));
    }
}
