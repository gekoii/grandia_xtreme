/* Third-party library code (newlib, fdlibm-derived): see README.md, "Third-party library code". */
#include "common.h"
#include "protos/main.h"
typedef union { f32 value; u32 word; } ieee_float_shape_type;
#define GET_FLOAT_WORD(i,d) do { ieee_float_shape_type gf_u; gf_u.value = (d); (i) = gf_u.word; } while (0)
#define SET_FLOAT_WORD(d,i) do { ieee_float_shape_type sf_u; sf_u.word = (i); (d) = sf_u.value; } while (0)


#define two25 3.355443200e+07f
#define twom25 2.9802322388e-08f
#define huge 1.0e+30f
#define tiny 1.0e-30f

f32 scalbnf(f32 x, s32 n) {
    s32 k, ix;
    GET_FLOAT_WORD(ix, x);
    k = (ix & 0x7f800000) >> 23;
    if (k == 0) {
        if ((ix & 0x7fffffff) == 0) return x;
        x *= two25;
        GET_FLOAT_WORD(ix, x);
        k = ((ix & 0x7f800000) >> 23) - 25;
        if (n < -50000) return tiny * x;
    }
    if (k == 0xff) return x + x;
    k = k + n;
    if (k > 0xfe) return huge * copysignf(huge, x);
    if (k > 0) {
        SET_FLOAT_WORD(x, (ix & 0x807fffff) | (k << 23));
        return x;
    }
    if (k <= -25) {
        if (n > 50000)
            return huge * copysignf(huge, x);
        else
            return tiny * copysignf(tiny, x);
    }
    k += 25;
    SET_FLOAT_WORD(x, (ix & 0x807fffff) | (k << 23));
    return x * twom25;
}
