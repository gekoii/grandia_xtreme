/* as: ee-gcc2.9-991111 */
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

#define GET_HIGH_WORD(i, d) do { ieee_double_shape_type gh_u; gh_u.value = (d); (i) = gh_u.parts.msw; } while (0)
#define EXTRACT_WORDS(ix0, ix1, d) do { ieee_double_shape_type ew_u; ew_u.value = (d); (ix0) = ew_u.parts.msw; (ix1) = ew_u.parts.lsw; } while (0)
#define SET_HIGH_WORD(d, v) do { ieee_double_shape_type sh_u; sh_u.value = (d); sh_u.parts.msw = (v); (d) = sh_u.value; } while (0)

double func_00481250(double, double);
double func_00481310(double, double);
extern double __scalbn_tiny;
extern double __scalbn_huge;

#define two54 1.80143985094819840000e+16
#define twom54 5.55111512312578270212e-17
#define huge __scalbn_huge
#define tiny __scalbn_tiny

double scalbn(double x, s32 n) {
    s32 k, hx, lx;
    EXTRACT_WORDS(hx, lx, x);
    k = (hx & 0x7ff00000) >> 20;
    if (k == 0) {
        if ((lx | (hx & 0x7fffffff)) == 0) return x;
        x = func_00481310(x, two54);
        GET_HIGH_WORD(hx, x);
        k = ((hx & 0x7ff00000) >> 20) - 54;
        if (n < -50000) return func_00481310(x, tiny);
    }
    if (k == 0x7ff) return func_00481250(x, x);
    k = k + n;
    if (k > 0x7fe) { double h = huge; return func_00481310(copysign(h, x), h); }
    if (k > 0) {
        SET_HIGH_WORD(x, (hx & 0x800fffff) | (k << 20));
        return x;
    }
    if (k <= -54) {
        if (n > 50000)
            { double h = huge; return func_00481310(copysign(h, x), h); }
        else
            { double h = tiny; return func_00481310(copysign(h, x), h); }
    }
    k += 54;
    SET_HIGH_WORD(x, (hx & 0x800fffff) | (k << 20));
    return func_00481310(x, twom54);
}
