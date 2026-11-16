/* cc: ee-gcc2.9-991111-01 */
/* as: ee-gcc2.9-991111-01 */
#include "common.h"
#include "field/n25.h"

f32 VuVecDot(f32 *, f32 *);
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

void MiniBallBounceBall(MiniBall *o, MiniBallHit *s)
{
    FVECTOR t;
    FVECTOR a;
    FVECTOR b;
    FVECTOR c;
    FVECTOR d;
    FVECTOR e;
    f32 f;
    f32 da;
    f32 db;
    f32 len;
    f32 k;

    o->lastWall = -1;
    o->lastBall = s->index;
    copyq(c, s->move);
    copyq(d, s->side);
    copyq(e, s->normal);
    vscale(t, o->vel, o->speed);
    f = s->dist;
    if (f < 0.0f) {
        f = 0.0f;
    }
    if (o->step >= 6) {
        if (f < 0.5f) {
            f = 0.5f;
        }
    }
    da = VuVecDot(t, d);
    db = -VuVecDot(t, e);
    vscale(a, d, da * miniBallData->params->ballTangentK);
    vscale(b, e, db * miniBallData->params->ballNormalK);
    vadd(t, a, b);
    o->travel = o->travel - f;
    if (o->travel < 0.0f) {
        o->travel = 0.0f;
    }
    vadd(o->pos, o->pos, c);
    len = VuVecNormalizeLen(o->vel, t);
    o->speed = len;
    o->travel = o->travel * miniBallData->params->ballNormalK;
    if (len > 1.5f) {
        k = 1.5f / len * 128.0f;
        if (k < miniBallWork->game.ballHitVol) {
            miniBallWork->game.ballHitVol = k;
            miniBallWork->game.ballHitSe = 1;
        }
    }
}
