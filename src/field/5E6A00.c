/* cc: ee-gcc2.9-991111-01 */
/* as: ee-gcc2.96-hazard */
#include "common.h"
#include "field/n17.h"

void VuVecCopy(f32 *, f32 *);
f32 VuVecDot(f32 *, f32 *);

static inline f32 fsqrt(f32 x)
{
    f32 r;
    __asm__ volatile("sqrt.s %0, %1" : "=f"(r) : "f"(x));
    return r;
}

#define VADD(o, a, b) \
    __asm__ volatile( \
        "lqc2 $vf1, 0(%1)\n\t" \
        "lqc2 $vf2, 0(%2)\n\t" \
        "vadd.xyz $vf1, $vf1, $vf2\n\t" \
        "sqc2 $vf1, 0(%0)" \
        : : "r"(o), "r"(a), "r"(b) : "memory")

#define VSUB(o, a, b) \
    __asm__ volatile( \
        "lqc2 $vf1, 0(%1)\n\t" \
        "lqc2 $vf2, 0(%2)\n\t" \
        "vsub.xyz $vf1, $vf1, $vf2\n\t" \
        "sqc2 $vf1, 0(%0)" \
        : : "r"(o), "r"(a), "r"(b) : "memory")

#define VSCALE(o, a, s) \
    __asm__ volatile( \
        "lqc2 $vf1, 0(%1)\n\t" \
        "qmtc2.ni %2, $vf2\n\t" \
        "vmulx.xyz $vf1, $vf1, $vf2x\n\t" \
        "sqc2 $vf1, 0(%0)" \
        : : "r"(o), "r"(a), "r"(s) : "memory")

s32 MapPolyWallSlide(f32 *pos, f32 *io, MapPolyTri *it, f32 r)
{
    FVECTOR x;
    FVECTOR a;
    FVECTOR b;
    FVECTOR c;
    FVECTOR d;
    FVECTOR n;
    f32 t;
    f32 s;
    f32 k;
    f32 z;

    VSUB(x, pos, it->v0);
    VuVecCopy(n, it->normal);
    do { /* do-while(0) + break: plain returns change the frame and layout */
        VSUB(d, pos, it->v0);
        if (VuVecDot(d, n) < 0.0f) {
            break;
        }
        VADD(a, io, d);
        if (r < VuVecDot(a, n)) {
            break;
        }
        t = VuVecDot(d, it->normal);
        if (t < r) {
            s = r - t;
            VuVecCopy(b, n);
            VSCALE(b, b, s);
            VuVecCopy(c, io);
        } else {
            VuVecCopy(a, io);
            s = __builtin_fabsf(VuVecDot(a, n));
            if (__builtin_fabsf(s) < 0.01f) {
                break;
            }
            s = (VuVecDot(d, n) - r) / s;
            VSCALE(b, io, s);
            s = 1.0f - s;
            VSCALE(c, io, s);
        }
        z = n[2];
        n[1] = 0.0f;
        s = n[0];
        n[2] = s;
        n[0] = -z;
        if (VuVecDot(c, n) < 0.0f) {
            VSCALE(n, n, -1.0f);
        }
        k = 1.0f / fsqrt(VuVecDot(n, n));
        k = k * (k * VuVecDot(n, c));
        VSCALE(c, n, k);
        io[0] = b[0] + c[0];
        io[2] = b[2] + c[2];
    } while (0);
    return 0;
}
