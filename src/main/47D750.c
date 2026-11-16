/* cflags: -fno-builtin */
/* cc: ee-gcc2.9-991111-01 */
/* as: ee-gcc2.9-991111-01 */
/* Third-party library code (newlib, fdlibm-derived): see README.md, "Third-party library code". */
#include "common.h"
#include "protos/main.h"

typedef union {
    f32 value;
    u32 word;
} ieee_float_shape_type;

#define GET_FLOAT_WORD(i, d)       \
    do {                           \
        ieee_float_shape_type gf_u; \
        gf_u.value = (d);          \
        (i) = gf_u.word;           \
    } while (0)

#define SET_FLOAT_WORD(d, i)       \
    do {                           \
        ieee_float_shape_type sf_u; \
        sf_u.word = (i);           \
        (d) = sf_u.value;          \
    } while (0)


extern const f32 __powf_bp[];
extern const f32 __powf_dp_h[];
extern const f32 __powf_dp_l[];
extern const f32 __powf_inf;

#define bp __powf_bp
#define dp_h __powf_dp_h
#define dp_l __powf_dp_l

#define zero ((f32)0.0)
#define one ((f32)1.0)
#define two ((f32)2.0)
#define two24 ((f32)16777216.0)
#define huge ((f32)1.0e30)
#define tiny ((f32)1.0e-30)
#define L1 ((f32)6.0000002384e-01)
#define L2 ((f32)4.2857143283e-01)
#define L3 ((f32)3.3333334327e-01)
#define L4 ((f32)2.7272811532e-01)
#define L5 ((f32)2.3066075146e-01)
#define L6 ((f32)2.0697501302e-01)
#define P1 ((f32)1.6666667163e-01)
#define P2 ((f32)-2.7777778450e-03)
#define P3 ((f32)6.6137559770e-05)
#define P4 ((f32)-1.6533901999e-06)
#define P5 ((f32)4.1381369442e-08)
#define lg2 ((f32)6.9314718246e-01)
#define lg2_h ((f32)6.93145752e-01)
#define lg2_l ((f32)1.42860654e-06)
#define ovt ((f32)4.2995665694e-08)
#define cp ((f32)9.6179670095e-01)
#define cp_h ((f32)9.6179199219e-01)
#define cp_l ((f32)4.7017383622e-06)
#define ivln2 ((f32)1.4426950216e+00)
#define ivln2_h ((f32)1.4426879883e+00)
#define ivln2_l ((f32)7.0526075433e-06)

f32 __ieee754_powf(f32 x, f32 y) {
    f32 z, ax, z_h, z_l, p_h, p_l;
    f32 y1, t1, t2, r, s, t, u, v, w;
    s32 i, j, k, yisint, n;
    s32 hx, hy, ix, iy, is;

    GET_FLOAT_WORD(hx, x);
    GET_FLOAT_WORD(hy, y);
    ix = hx & 0x7fffffff;
    iy = hy & 0x7fffffff;

    if (iy == 0) return one;

    if (ix > 0x7f800000 || iy > 0x7f800000) return x + y;

    yisint = 0;
    if (hx < 0) {
        if (iy >= 0x4b800000) yisint = 2;
        else if (iy >= 0x3f800000) {
            k = (iy >> 23) - 0x7f;
            j = iy >> (23 - k);
            if ((j << (23 - k)) == iy) yisint = 2 - (j & 1);
        }
    }

    if (iy == 0x7f800000) {
        if (ix == 0x3f800000)
            return y - y;
        else if (ix > 0x3f800000)
            return (hy >= 0) ? y : zero;
        else
            return (hy < 0) ? -y : zero;
    }
    if (iy == 0x3f800000) {
        if (hy < 0) return one / x;
        else return x;
    }
    if (hy == 0x40000000) return x * x;
    if (hy == 0x3f000000) {
        if (hx >= 0) return __ieee754_sqrtf(x);
    }

    ax = fabsf(x);
    if (ix == 0x7f800000 || ix == 0 || ix == 0x3f800000) {
        z = ax;
        if (hy < 0) z = one / z;
        if (hx < 0) {
            if (((ix - 0x3f800000) | yisint) == 0) {
                z = (z - z) / (z - z);
            } else if (yisint == 1)
                z = -z;
        }
        return z;
    }

    if (((((u32)hx >> 31) - 1) | yisint) == 0) return (x - x) / (x - x);

    if (iy > 0x4d000000) {
        if (ix < 0x3f7ffff8) return (hy < 0) ? __powf_inf : tiny * tiny;
        if (ix > 0x3f800007) return (hy > 0) ? __powf_inf : tiny * tiny;
        t = x - 1;
        w = (t * t) * ((f32)0.5 - t * ((f32)0.333333333333 - t * (f32)0.25));
        u = ivln2_h * t;
        v = t * ivln2_l - w * ivln2;
        t1 = u + v;
        GET_FLOAT_WORD(is, t1);
        SET_FLOAT_WORD(t1, is & 0xfffff000);
        t2 = v - (t1 - u);
    } else {
        f32 s2, s_h, s_l, t_h, t_l;
        n = 0;
        if (ix < 0x00800000) {
            ax *= two24;
            n -= 24;
            GET_FLOAT_WORD(ix, ax);
        }
        n += ((ix) >> 23) - 0x7f;
        j = ix & 0x007fffff;
        ix = j | 0x3f800000;
        if (j <= 0x1cc471) k = 0;
        else if (j < 0x5db3d7) k = 1;
        else {
            k = 0;
            n += 1;
            ix -= 0x00800000;
        }
        SET_FLOAT_WORD(ax, ix);

        u = ax - bp[k];
        v = one / (ax + bp[k]);
        s = u * v;
        s_h = s;
        GET_FLOAT_WORD(is, s_h);
        SET_FLOAT_WORD(s_h, is & 0xfffff000);
        SET_FLOAT_WORD(t_h, ((ix >> 1) | 0x20000000) + 0x0040000 + (k << 21));
        t_l = ax - (t_h - bp[k]);
        s_l = v * ((u - s_h * t_h) - s_h * t_l);
        s2 = s * s;
        r = s2 * s2 * (L1 + s2 * (L2 + s2 * (L3 + s2 * (L4 + s2 * (L5 + s2 * L6)))));
        r += s_l * (s_h + s);
        s2 = s_h * s_h;
        t_h = (f32)3.0 + s2 + r;
        GET_FLOAT_WORD(is, t_h);
        SET_FLOAT_WORD(t_h, is & 0xfffff000);
        t_l = r - ((t_h - (f32)3.0) - s2);
        u = s_h * t_h;
        v = s_l * t_h + t_l * s;
        p_h = u + v;
        GET_FLOAT_WORD(is, p_h);
        SET_FLOAT_WORD(p_h, is & 0xfffff000);
        p_l = v - (p_h - u);
        z_h = cp_h * p_h;
        z_l = cp_l * p_h + p_l * cp + dp_l[k];
        t = (f32)n;
        t1 = (((z_h + z_l) + dp_h[k]) + t);
        GET_FLOAT_WORD(is, t1);
        SET_FLOAT_WORD(t1, is & 0xfffff000);
        t2 = z_l - (((t1 - t) - dp_h[k]) - z_h);
    }

    s = one;
    if (((((u32)hx >> 31) - 1) | (yisint - 1)) == 0)
        s = -one;

    GET_FLOAT_WORD(is, y);
    SET_FLOAT_WORD(y1, is & 0xfffff000);
    p_l = (y - y1) * t1 + y * t2;
    p_h = y1 * t1;
    z = p_l + p_h;
    GET_FLOAT_WORD(j, z);
    if (j > 0x43000000)
        return s * huge * huge;
    else if (j == 0x43000000) {
        if (p_l + ovt > z - p_h) return s * huge * huge;
    } else if ((j & 0x7fffffff) > 0x43160000)
        return s * tiny * tiny;
    else if ((u32)j == 0xc3160000) {
        if (p_l <= z - p_h) return s * tiny * tiny;
    }
    i = j & 0x7fffffff;
    k = (i >> 23) - 0x7f;
    n = 0;
    if (i > 0x3f000000) {
        n = j + (0x00800000 >> (k + 1));
        k = ((n & 0x7fffffff) >> 23) - 0x7f;
        SET_FLOAT_WORD(t, n & ~(0x007fffff >> k));
        n = ((n & 0x007fffff) | 0x00800000) >> (23 - k);
        if (j < 0) n = -n;
        p_h -= t;
    }
    t = p_l + p_h;
    GET_FLOAT_WORD(is, t);
    SET_FLOAT_WORD(t, is & 0xfffff000);
    u = t * lg2_h;
    v = (p_l - (t - p_h)) * lg2 + t * lg2_l;
    z = u + v;
    w = v - (z - u);
    t = z * z;
    t1 = z - t * (P1 + t * (P2 + t * (P3 + t * (P4 + t * P5))));
    r = (z * t1) / (t1 - two) - (w + z * w);
    z = one - (r - z);
    GET_FLOAT_WORD(j, z);
    j += (n << 23);
    if ((j >> 23) <= 0) z = scalbnf(z, n);
    else SET_FLOAT_WORD(z, j);
    return s * z;
}
