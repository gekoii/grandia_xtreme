/* Third-party library code (newlib, fdlibm-derived): see README.md, "Third-party library code". */
#include "common.h"

typedef union {
    f32 value;
    u32 word;
} ieee_float_shape_type;

#define GET_FLOAT_WORD(i, d) \
    do { \
        ieee_float_shape_type gf_u; \
        gf_u.value = (d); \
        (i) = gf_u.word; \
    } while (0)

#define SET_FLOAT_WORD(d, i) \
    do { \
        ieee_float_shape_type sf_u; \
        sf_u.word = (i); \
        (d) = sf_u.value; \
    } while (0)

f32 floorf(f32 x) {
    s32 i0, j0;
    u32 i;

    GET_FLOAT_WORD(i0, x);
    j0 = ((i0 >> 23) & 0xff) - 0x7f;
    if (j0 < 23) {
        if (j0 < 0) {
            if (1.0e30f + x > 0.0f) {
                if (i0 >= 0) {
                    i0 = 0;
                } else if ((i0 & 0x7fffffff) != 0) {
                    i0 = 0xbf800000;
                }
            }
        } else {
            i = (0x007fffff) >> j0;
            if ((i0 & i) == 0) {
                return x;
            }
            if (1.0e30f + x > 0.0f) {
                if (i0 < 0) {
                    i0 += (0x00800000) >> j0;
                }
                i0 &= (~i);
            }
        }
    } else {
        if (j0 == 0x80) {
            return x + x;
        } else {
            return x;
        }
    }
    SET_FLOAT_WORD(x, i0);
    return x;
}
