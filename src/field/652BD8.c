/* as: ee-gcc2.9-991111-01 */
#include "common.h"
#include "protos/field.h"
#include "field/n25.h"

f32 VuVecDot(f32 *, f32 *);
void VuVecNormalize(f32 *, f32 *);

#define copyq(dst, src) \
    __asm__ volatile("lq $2, 0(%1)\n\tsq $2, 0(%0)" : : "r"(dst), "r"(src) : "$2", "memory")

#define vscale(dst, src, k) \
    __asm__ volatile( \
        "lqc2 $vf1, 0(%1)\n\t" \
        "qmtc2.ni %2, $vf2\n\t" \
        "vmulx.xyz $vf1, $vf1, $vf2x\n\t" \
        "sqc2 $vf1, 0(%0)" \
        : : "r"(dst), "r"(src), "r"(k) : "memory")

#define vsub(dst, a, b) \
    __asm__ volatile( \
        "lqc2 $vf1, 0(%1)\n\t" \
        "lqc2 $vf2, 0(%2)\n\t" \
        "vsub.xyz $vf1, $vf1, $vf2\n\t" \
        "sqc2 $vf1, 0(%0)" \
        : : "r"(dst), "r"(a), "r"(b) : "memory")

static inline f32 fsqrt(f32 x)
{
    f32 r;
    __asm__ volatile("sqrt.s %0, %1" : "=f"(r) : "f"(x));
    return r;
}

void MiniBallSweepBalls(MiniBall *arg0, MiniBallHit *arg1)
{
    FVECTOR t;
    FVECTOR b;
    FVECTOR a;
    FVECTOR pos;
    FVECTOR d;
    FVECTOR n;
    FVECTOR e;
    FVECTOR r;
    FVECTOR f;
    f32 rad;
    f32 len;
    f32 dist;
    u32 i;
    MiniBall *c;

    rad = arg0->travel;
    copyq(a, arg0->pos);
    copyq(b, arg0->vel);
    vscale(t, b, rad);
    c = miniBallWork->game.balls - 1;
    for (i = 0; i < 20; i++) {
        c++;
        if (i == arg0->index) continue;
        if (i == arg0->lastBall) continue;
        if (c->state != 4) continue;
        copyq(pos, c->pos);
        vsub(d, pos, a);
        if (VuVecDot(d, t) < 0.0f) continue;
        len = fsqrt(d[0] * d[0] + d[1] * d[1] + d[2] * d[2]);
        if (rad + 20.0f < len) continue;
        if (20.0f < len) {
            dist = MiniBallSweepCircle(d, b, 20.0f);
        } else {
            dist = len - 20.0f;
        }
        if (arg1->dist <= dist) continue;
        if (dist <= 0.0f) {
            copyq(n, miniBallWork->vec0);
        } else {
            vscale(n, b, dist);
        }
        vsub(e, d, n);
        VuVecNormalize(f, e);
        copyq(r, miniBallWork->vec0);
        r[0] = f[2];
        r[2] = -f[0];
        VuVecNormalize(r, r);
        arg1->dist = dist;
        arg1->kind = 1;
        arg1->index = i;
        copyq(arg1->move, n);
        copyq(arg1->side, r);
        copyq(arg1->normal, f);
    }
}
