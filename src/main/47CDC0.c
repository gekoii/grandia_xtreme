/* cflags: -fno-builtin */
/* cc: ee-gcc2.9-991111-01 */
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

#define SET_FLOAT_WORD(d, i)            \
    do {                                \
        ieee_float_shape_type sf_u;     \
        sf_u.word = (i);                \
        (d) = sf_u.value;               \
    } while (0)

extern const f32 __atan2f_mzero;

#define tiny 1.0e-30f
#define zero 0.0f
#define pi_o_4 7.8539818525e-01f
#define pi_o_2 1.5707963705e+00f
#define pi 3.1415925026e+00f
#define pi_lo 1.5099578832e-07f

f32 __ieee754_atan2f(f32 y, f32 x) {
    f32 z;
    s32 k, m, hx, hy, ix, iy;

    GET_FLOAT_WORD(hx, x);
    ix = hx & 0x7fffffff;
    GET_FLOAT_WORD(hy, y);
    iy = hy & 0x7fffffff;
    if (ix > 0x7f800000 || iy > 0x7f800000) {
        return x + y;
    }
    if (hx == 0x3f800000) {
        return atanf(y);
    }
    m = ((hy >> 31) & 1) | ((hx >> 30) & 2);

    if (iy == 0) {
        switch (m) {
        case 0:
        case 1:
            return y;
        case 2:
            return pi + tiny;
        case 3:
            return -pi - tiny;
        }
    }
    if (ix == 0) {
        return (hy < 0) ? -pi_o_2 - tiny : pi_o_2 + tiny;
    }
    if (ix == 0x7f800000) {
        if (iy == 0x7f800000) {
            switch (m) {
            case 0:
                return pi_o_4 + tiny;
            case 1:
                return -pi_o_4 - tiny;
            case 2:
                return (f32)3.0 * pi_o_4 + tiny;
            case 3:
                return (f32)-3.0 * pi_o_4 - tiny;
            }
        } else {
            switch (m) {
            case 0:
                return zero;
            case 1:
                return __atan2f_mzero;
            case 2:
                return pi + tiny;
            case 3:
                return -pi - tiny;
            }
        }
    }
    if (iy == 0x7f800000) {
        return (hy < 0) ? -pi_o_2 - tiny : pi_o_2 + tiny;
    }

    k = (iy - ix) >> 23;
    if (k > 60) {
        z = pi_o_2 + (f32)0.5 * pi_lo;
    } else if (hx < 0 && k < -60) {
        z = 0.0;
    } else {
        z = atanf(fabsf(y / x));
    }
    switch (m) {
    case 0:
        return z;
    case 1: {
        u32 zh;
        GET_FLOAT_WORD(zh, z);
        SET_FLOAT_WORD(z, zh ^ 0x80000000);
    }
        return z;
    case 2:
        return pi - (z - pi_lo);
    default:
        return (z - pi_lo) - pi;
    }
}
