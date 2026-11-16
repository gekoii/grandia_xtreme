/* cc: ee-gcc2.9-991111-01 */
/* Matches newlib's __ieee754_asinf (ef_asin.c). Third-party library code: see README.md, "Third-party library code". */
#include "common.h"
#include "protos/viewer.h"

typedef union {
    f32 value;
    u32 word;
} ieee_float_shape_type;

#define GET_FLOAT_WORD(i, d)          \
    do {                              \
        ieee_float_shape_type gf_u;   \
        gf_u.value = (d);             \
        (i) = gf_u.word;              \
    } while (0)

#define SET_FLOAT_WORD(d, i)          \
    do {                              \
        ieee_float_shape_type sf_u;   \
        sf_u.word = (i);              \
        (d) = sf_u.value;             \
    } while (0)


f32 vwr___ieee754_asinf(f32 x) {
    f32 t, w, p, q, c, r, s;
    s32 hx, ix;

    GET_FLOAT_WORD(hx, x);
    ix = hx & 0x7fffffff;
    if (ix == 0x3f800000) {
        return x * 1.5707962513e+00f + x * 7.5497894159e-08f;
    } else if (ix > 0x3f800000) {
        return (x - x) / (x - x);
    } else if (ix < 0x3f000000) {
        if (ix < 0x32000000) {
            if (1.0e30f + x > 1.0f)
                return x;
        } else
            t = x * x;
        p = t * (1.6666667163e-01f + t * (-3.2556581497e-01f + t * (2.0121252537e-01f + t * (-4.0055535734e-02f + t * (7.9153501429e-04f + t * 3.4793309169e-05f)))));
        q = 1.0f + t * (-2.4033949375e+00f + t * (2.0209457874e+00f + t * (-6.8828397989e-01f + t * 7.7038154006e-02f)));
        w = p / q;
        return x + x * w;
    }
    w = 1.0f - vwr_fabsf(x);
    t = w * 0.5f;
    p = t * (1.6666667163e-01f + t * (-3.2556581497e-01f + t * (2.0121252537e-01f + t * (-4.0055535734e-02f + t * (7.9153501429e-04f + t * 3.4793309169e-05f)))));
    q = 1.0f + t * (-2.4033949375e+00f + t * (2.0209457874e+00f + t * (-6.8828397989e-01f + t * 7.7038154006e-02f)));
    s = vwr___ieee754_sqrtf(t);
    if (ix >= 0x3F79999A) {
        w = p / q;
        t = 1.5707962513e+00f - (2.0f * (s + s * w) - 7.5497894159e-08f);
    } else {
        s32 iw;
        w = s;
        GET_FLOAT_WORD(iw, w);
        SET_FLOAT_WORD(w, iw & 0xfffff000);
        c = (t - w * w) / (s + w);
        r = p / q;
        p = 2.0f * s * r - (7.5497894159e-08f - 2.0f * c);
        q = 7.8539818525e-01f - 2.0f * w;
        t = 7.8539818525e-01f - (p - q);
    }
    if (hx > 0)
        return t;
    else
        return -t;
}
