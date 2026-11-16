/* cc: ee-gcc2.9-991111-01 */
#include "common.h"
#include "gph.h"
#include "camera.h"
#include "protos/main.h"

void VuMtxGetScale(f32 *, f32 *, f32 *);
void VuMtxScale(f32 *, f32 *);

static inline f32 fsqrt(f32 x)
{
    f32 r;
    __asm__ volatile("sqrt.s %0, %1" : "=f"(r) : "f"(x));
    return r;
}

#define copyq(dst, src) __asm__ volatile("lq $2, 0(%1)\n\tsq $2, 0(%0)" : : "r"(dst), "r"(src) : "$2", "memory")

void GphPartBillboard(u32 mode, f32 *m)
{
    FVECTOR scale;
    FVECTOR t;
    FVECTOR tmp[4];
    f32 x;
    f32 y;
    f32 z;
    f32 xx;
    f32 zz;
    s32 a;
    s32 b;
    s32 c;
    Camera *cam;
    f32 l;
    f32 len;
    f32 *v;

    if (mode < 3) {
        VuMtxGetScale(scale, (f32 *)tmp, m);
        v = &m[12];
        copyq(t, v);
        __asm__ volatile(
            "vmr32.xyzw $vf5, $vf0\n\t"
            "vmr32.xyzw $vf6, $vf5\n\t"
            "vmr32.xyzw $vf7, $vf6\n\t"
            "sqc2 $vf0, 0x30(%0)\n\t"
            "sqc2 $vf5, 0x20(%0)\n\t"
            "sqc2 $vf6, 0x10(%0)\n\t"
            "sqc2 $vf7, 0x0(%0)"
            : : "r"(m) : "memory");
        VuMtxScale(m, scale);
        x = t[0];
        y = t[1];
        z = t[2];
        switch (mode) {
        case 0:
            zz = z * z;
            xx = x * x;
            l = fsqrt(xx + zz);
            a = asinf(-x / l) * 10430.378f;
            xx = xx + y * y;
            l = xx + zz;
            b = asinf(y / fsqrt(l)) * -10430.378f;
            VuMtxRotY(m, a, m);
            VuMtxRotX(m, b, m);
            break;
        case 1:
            l = x * x + z * z;
            len = fsqrt(l);
            a = asinf(-x / len) * 10430.378f;
            cam = gphRenderCtx->camera;
            b = cam->pitch * -10430.378f;
            c = cam->roll * -10430.378f;
            VuMtxRotY(m, a, m);
            VuMtxRotX(m, b, m);
            VuMtxRotZ(m, c, m);
            break;
        case 2:
            zz = z * z;
            xx = x * x;
            l = fsqrt(xx + zz);
            a = asinf(-x / l) * 10430.378f;
            xx = xx + y * y;
            l = xx + zz;
            b = asinf(y / fsqrt(l)) * -10430.378f;
            cam = gphRenderCtx->camera;
            c = cam->roll * -10430.378f;
            VuMtxRotY(m, a, m);
            VuMtxRotX(m, b, m);
            VuMtxRotZ(m, c, m);
            break;
        }
        __asm__ volatile(
            "lqc2 $vf1, 0x30(%1)\n\t"
            "lqc2 $vf2, 0x0(%2)\n\t"
            "lqc2 $vf3, 0x0(%1)\n\t"
            "lqc2 $vf4, 0x10(%1)\n\t"
            "lqc2 $vf5, 0x20(%1)\n\t"
            "vadd.xyz $vf1, $vf1, $vf2\n\t"
            "sqc2 $vf3, 0x0(%0)\n\t"
            "sqc2 $vf4, 0x10(%0)\n\t"
            "sqc2 $vf5, 0x20(%0)\n\t"
            "sqc2 $vf1, 0x30(%0)"
            : : "r"(m), "r"(m), "r"(t) : "memory");
    } else {
        switch (mode) {
        case 3:
            __asm__ volatile(
                "vmr32.xyzw $vf5, $vf0\n\t"
                "vmr32.xyzw $vf6, $vf5\n\t"
                "vmr32.xyzw $vf7, $vf6\n\t"
                "sqc2 $vf0, 0x30(%0)\n\t"
                "sqc2 $vf5, 0x20(%0)\n\t"
                "sqc2 $vf6, 0x10(%0)\n\t"
                "sqc2 $vf7, 0x0(%0)"
                : : "r"(m) : "memory");
            break;
        case 4:
            v = &m[12];
            v[0] = 0.0f;
            v[3] = 1.0f;
            v[2] = 0.0f;
            v[1] = 0.0f;
            break;
        }
    }
}
