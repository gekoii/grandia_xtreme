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

#define EXTRACT_WORDS(ix0, ix1, d) \
    do { ieee_double_shape_type ew_u; ew_u.value = (d); (ix0) = ew_u.parts.msw; (ix1) = ew_u.parts.lsw; } while (0)
#define GET_HIGH_WORD(i, d) \
    do { ieee_double_shape_type gh_u; gh_u.value = (d); (i) = gh_u.parts.msw; } while (0)
#define SET_HIGH_WORD(d, v) \
    do { ieee_double_shape_type sh_u; sh_u.value = (d); sh_u.parts.msw = (v); (d) = sh_u.value; } while (0)
#define INSERT_WORDS(d, ix0, ix1) \
    do { ieee_double_shape_type iw_u; iw_u.parts.msw = (ix0); iw_u.parts.lsw = (ix1); (d) = iw_u.value; } while (0)

double func_004812A8(double, double);
extern double TWO52[2];

double rint(double x) {
    s32 i0, j0, sx;
    u32 i, i1;
    double c, w, t;

    EXTRACT_WORDS(i0, i1, x);
    sx = (i0 >> 31) & 1;
    j0 = ((i0 >> 20) & 0x7ff) - 0x3ff;
    if (j0 < 20) {
        if (j0 < 0) {
            double c2;
            if (((i0 & 0x7fffffff) | i1) == 0) {
                return x;
            }
            i1 |= (i0 & 0x0fffff);
            i0 &= 0xfffe0000;
            i0 |= ((i1 | -i1) >> 12) & 0x80000;
            SET_HIGH_WORD(x, i0);
            c2 = TWO52[sx];
            w = dpadd(c2, x);
            t = func_004812A8(w, c2);
            GET_HIGH_WORD(i0, t);
            SET_HIGH_WORD(t, (i0 & 0x7fffffff) | (sx << 31));
            return t;
        } else {
            i = (0x000fffff) >> j0;
            if (((i0 & i) | i1) == 0) {
                return x;
            }
            i >>= 1;
            if (((i0 & i) | i1) != 0) {
                if (j0 == 19) {
                    i1 = 0x40000000;
                } else {
                    i0 = (i0 & (~i)) | ((0x20000) >> j0);
                }
            }
        }
    } else if (j0 > 51) {
        if (j0 == 0x400) {
            return dpadd(x, x);
        } else {
            return x;
        }
    } else {
        i = ((u32)(0xffffffff)) >> (j0 - 20);
        if ((i1 & i) == 0) {
            return x;
        }
        i >>= 1;
        if ((i1 & i) != 0) {
            i1 = (i1 & (~i)) | ((0x40000000) >> (j0 - 20));
        }
    }
    INSERT_WORDS(x, i0, i1);
    c = TWO52[sx];
    w = dpadd(c, x);
    return func_004812A8(w, c);
}
