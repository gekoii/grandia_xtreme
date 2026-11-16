/* Matches newlib's isnanf (sf_isnan.c). Third-party library code: see README.md, "Third-party library code". */
#include "common.h"

typedef union {
    f32 value;
    u32 word;
} ieee_float_shape_type;

#define GET_FLOAT_WORD(i, d) do { ieee_float_shape_type gf_u; gf_u.value = (d); (i) = gf_u.word; } while (0)

s32 vwr_isnanf(f32 x) {
    s32 ix;
    GET_FLOAT_WORD(ix, x);
    ix &= 0x7fffffff;
    ix = 0x7f800000 - ix;
    return (s32)(((u32)(ix)) >> 31);
}
