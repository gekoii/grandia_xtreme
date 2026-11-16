/* as: ee-gcc2.9-991111-01 */
#include "common.h"
#include "pad.h"
#include "protos/field.h"
#include "field/n25.h"

f32 VuVecNormalizeLen(f32 *, f32 *);

#define copyq(dst, src) \
    __asm__ volatile("lq $2, 0(%1)\n\tsq $2, 0(%0)" : : "r"(dst), "r"(src) : "$2", "memory")

#define vscale(dst, src, k) \
    __asm__ volatile( \
        "lqc2 $vf1, 0(%1)\n\t" \
        "qmtc2.ni %2, $vf2\n\t" \
        "vmulx.xyz $vf1, $vf1, $vf2x\n\t" \
        "sqc2 $vf1, 0(%0)" \
        : : "r"(dst), "r"(src), "r"(k) : "memory")

#define vadd(dst, a, b) \
    __asm__ volatile( \
        "lqc2 $vf1, 0(%1)\n\t" \
        "lqc2 $vf2, 0(%2)\n\t" \
        "vadd.xyz $vf1, $vf1, $vf2\n\t" \
        "sqc2 $vf1, 0(%0)" \
        : : "r"(dst), "r"(a), "r"(b) : "memory")

void MiniBallShakeUpdate(void)
{
    FVECTOR p;
    FVECTOR v;
    FVECTOR a;
    FVECTOR b;
    FVECTOR c;
    u32 i;
    f32 d;
    s32 ix;
    s32 iz;
    MiniPanel *e;

    copyq(p, miniBallWork->game.shakePos);
    copyq(v, miniBallWork->game.shakeVel);
    vscale(c, p, -0.8f);
    vadd(a, miniBallWork->game.tilt, c);
    vscale(b, a, 3.2f);
    vadd(v, v, b);
    vadd(p, p, v);
    vscale(v, v, 0.9f);
    for (i = 0; i < 4; i++) {
        ix = p[0];
        iz = p[2];
        e = &miniBallWork->game.panels[1 + i];
        e->x = ix;
        e->y = iz;
    }
    copyq(miniBallWork->game.shakePos, p);
    copyq(miniBallWork->game.shakeVel, v);
    copyq(miniBallWork->game.tilt, miniBallWork->vec0);
    d = VuVecNormalizeLen(p, p);
    if (d > 1.0f) {
        d *= 76.5f;
        if (d > 255.0f) {
            d = 255.0f;
        }
        PadVibrate(padPort0, 1, (s32)d, 1);
    }
}
