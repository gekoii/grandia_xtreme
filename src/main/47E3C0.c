/* cc: ee-gcc2.9-991111-01 */
/* rodata: 0x004A80B0-0x004A80B8 */
/* Matches newlib's __ieee754_sqrtf (ef_sqrt.c). Third-party library code: see README.md, "Third-party library code". */
#include "common.h"

typedef union {
    f32 value;
    u32 word;
} ieee_float_shape_type;

#define GET_FLOAT_WORD(i, d) do { ieee_float_shape_type gf_u; gf_u.value = (d); (i) = gf_u.word; } while (0)
#define SET_FLOAT_WORD(d, i) do { ieee_float_shape_type sf_u; sf_u.word = (i); (d) = sf_u.value; } while (0)

static const f32 one = 1.0, tiny = 1.0e-30;

f32 __ieee754_sqrtf(f32 x) {
    f32 z;
    s32 sign = (s32)0x80000000;
    u32 r;
    s32 ix, s, q, m, t, i;

    GET_FLOAT_WORD(ix, x);

    if ((ix & 0x7f800000) == 0x7f800000) {
        return x * x + x;
    }
    if (ix <= 0) {
        if ((ix & (~sign)) == 0) return x;
        else if (ix < 0)
            return (x - x) / (x - x);
    }
    m = (ix >> 23);
    if (m == 0) {
        for (i = 0; (ix & 0x00800000) == 0; i++) ix <<= 1;
        m -= i - 1;
    }
    m -= 127;
    ix = (ix & 0x007fffff) | 0x00800000;
    if (m & 1)
        ix += ix;
    m >>= 1;

    ix += ix;
    q = s = 0;
    r = 0x01000000;

    while (r != 0) {
        t = s + r;
        if (t <= ix) {
            s = t + r;
            ix -= t;
            q += r;
        }
        ix += ix;
        r >>= 1;
    }

    if (ix != 0) {
        z = one - tiny;
        if (z >= one) {
            z = one + tiny;
            if (z > one)
                q += 2;
            else
                q += (q & 1);
        }
    }
    ix = (q >> 1) + 0x3f000000;
    ix += (m << 23);
    SET_FLOAT_WORD(z, ix);
    return z;
}
