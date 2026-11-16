/* as: ee-gcc2.9-991111-01 */
/* rodata: 0x004A826C-0x004A8288 */
/* Matches newlib's __kernel_sinf (kf_sin.c). Third-party library code: see README.md, "Third-party library code". */
#include "common.h"

typedef union {
    f32 value;
    u32 word;
} ieee_float_shape_type;

#define GET_FLOAT_WORD(i, d) do { ieee_float_shape_type gf_u; gf_u.value = (d); (i) = gf_u.word; } while (0)

static const f32
half = 5.0000000000e-01,
S1 = -1.6666667163e-01,
S2 = 8.3333337680e-03,
S3 = -1.9841270114e-04,
S4 = 2.7557314297e-06,
S5 = -2.5050759689e-08,
S6 = 1.5896910177e-10;

f32 __kernel_sinf(f32 x, f32 y, int iy) {
    f32 z, r, v;
    s32 ix;
    GET_FLOAT_WORD(ix, x);
    ix &= 0x7fffffff;
    if (ix < 0x32000000) {
        if ((int)x == 0) return x;
    }
    z = x * x;
    v = z * x;
    r = S2 + z * (S3 + z * (S4 + z * (S5 + z * S6)));
    if (iy == 0) return x + v * (S1 + z * r);
    else return x - ((z * (half * y - v * r) - y) - v * S1);
}
