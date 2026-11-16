/* Third-party library code (newlib, fdlibm-derived): see README.md, "Third-party library code". */
#include "common.h"
typedef union { f32 value; u32 word; } ieee_float_shape_type;
#define GET_FLOAT_WORD(i,d) do { ieee_float_shape_type gf_u; gf_u.value = (d); (i) = gf_u.word; } while (0)
#define SET_FLOAT_WORD(d,i) do { ieee_float_shape_type sf_u; sf_u.word = (i); (d) = sf_u.value; } while (0)
s32 finitef(f32 x) {
    u32 ix;
    GET_FLOAT_WORD(ix, x);
    return (s32)((u32)((ix & 0x7fffffff) - 0x7f800000) >> 31);
}
