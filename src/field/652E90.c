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

void MiniBallBounceWall(MiniBall *arg0, MiniBallHit *arg1)
{
    FVECTOR t;
    FVECTOR u;
    FVECTOR v;
    FVECTOR d;
    FVECTOR p;
    FVECTOR q;
    f32 a;
    f32 b;
    f32 m;
    f32 x;

    arg0->lastWall = arg1->index;
    arg0->lastBall = -1;
    copyq(d, arg1->move);
    copyq(p, arg1->side);
    copyq(q, arg1->normal);
    vscale(t, arg0->vel, arg0->speed);
    a = VuVecDot(t, p);
    b = -VuVecDot(t, q);
    m = arg1->dist;
    if (m < 0.0f) {
        m = 0.0f;
    }
    if (arg0->step >= 6) {
        if (m < 0.5f) {
            m = 0.5f;
        }
    }
    vscale(u, p, a * miniBallData->params->wallTangentK);
    vscale(v, q, b * miniBallData->params->wallNormalK);
    vadd(t, u, v);
    arg0->travel = arg0->travel - m;
    if (arg0->travel < 0.0f) {
        arg0->travel = 0.0f;
    }
    vadd(arg0->pos, arg0->pos, d);
    x = VuVecNormalizeLen(arg0->vel, t);
    arg0->speed = x;
    arg0->travel = arg0->travel * miniBallData->params->wallNormalK;
    if (1.5f < x) {
        x = 1.5f / x * 128.0f;
        if (x < miniBallWork->game.wallHitVol) {
            miniBallWork->game.wallHitVol = (s32)x;
            miniBallWork->game.wallHitSe = 1;
        }
    }
}
