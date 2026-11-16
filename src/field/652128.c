/* cc: ee-gcc2.9-991111-01 */
/* as: ee-gcc2.9-991111-01 */
#include "common.h"
#include "field/n25.h"

f32 VuVecDot(f32 *, f32 *);
void VuVecNormalize(f32 *, f32 *);
f32 VuVecNormalizeLen(f32 *, f32 *);

static inline void copyq(void *dst, void *src) {
    __asm__ volatile("lq $2, 0(%1)\n\tsq $2, 0(%0)" : : "r"(dst), "r"(src) : "$2", "memory");
}

#define VSCALE(dst, src, s) \
    __asm__ volatile( \
        "lqc2 $vf1, 0(%1)\n\t" \
        "qmtc2.ni %2, $vf2\n\t" \
        "vmulx.xyz $vf1, $vf1, $vf2x\n\t" \
        "sqc2 $vf1, 0(%0)" \
        : : "r"(dst), "r"(src), "r"(s) : "memory")

#define VADD(dst, a, b) \
    __asm__ volatile( \
        "lqc2 $vf1, 0(%1)\n\t" \
        "lqc2 $vf2, 0(%2)\n\t" \
        "vadd.xyz $vf1, $vf1, $vf2\n\t" \
        "sqc2 $vf1, 0(%0)" \
        : : "r"(dst), "r"(a), "r"(b) : "memory")

#define VSUB(dst, a, b) \
    __asm__ volatile( \
        "lqc2 $vf1, 0(%1)\n\t" \
        "lqc2 $vf2, 0(%2)\n\t" \
        "vsub.xyz $vf1, $vf1, $vf2\n\t" \
        "sqc2 $vf1, 0(%0)" \
        : : "r"(dst), "r"(a), "r"(b) : "memory")

void MiniBallCollideBalls(MiniBall *w, u8 n)
{
    FVECTOR vi;
    FVECTOR t1;
    FVECTOR m1;
    FVECTOR vj;
    FVECTOR t2;
    FVECTOR m2;
    FVECTOR pi;
    FVECTOR pj;
    FVECTOR d;
    FVECTOR ei;
    FVECTOR ej;
    FVECTOR e;
    FVECTOR up;
    FVECTOR nd;
    FVECTOR side;
    FVECTOR ne;
    u32 i;
    u32 j;

    copyq(up, miniBallWork->vec0);
    for (i = 0; i < n - 1; i++) {
        if (w[i].state == 4) {
            copyq(pi, w[i].pos);
            VSCALE(vi, w[i].vel, w[i].speed);
            for (j = i + 1; j < n; j++) {
                if (w[j].state == 4) {
                    f32 a, b, c, f;
                    copyq(pj, w[j].pos);
                    VSCALE(vj, w[j].vel, w[j].speed);
                    VSUB(d, pj, pi);
                    {
                        f32 da, db;
                        da = VuVecDot(vi, d);
                        db = VuVecDot(vj, d);
                        if (da < 0.00001f && db > -0.00001f) {
                            continue;
                        }
                    }
                    VADD(ei, pi, vi);
                    VADD(ej, pj, vj);
                    VSUB(e, ej, ei);
                    {
                        f32 la, lb;
                        la = VuVecNormalizeLen(ne, e);
                        lb = VuVecNormalizeLen(nd, d);
                        if (lb < la) {
                            continue;
                        }
                        if (lb > 20.0f && la > 20.0f) {
                            continue;
                        }
                    }
                    up[0] = d[2];
                    up[2] = -d[0];
                    VuVecNormalize(side, up);
                    a = VuVecDot(vi, side);
                    b = VuVecDot(vi, nd);
                    c = VuVecDot(vj, side);
                    f = VuVecDot(vj, nd);
                    VSCALE(m1, side, a * miniBallData->params->ballTangentK);
                    VSCALE(t1, nd, f * miniBallData->params->ballNormalK);
                    VSCALE(m2, side, c * miniBallData->params->ballTangentK);
                    VSCALE(t2, nd, b * miniBallData->params->ballNormalK);
                    VADD(vi, m1, t1);
                    VADD(vj, m2, t2);
                    w[j].travel = w[j].speed = VuVecNormalizeLen(w[j].vel, vj);
                    if (w[i].speed > 1.5f) {
                        f32 f;
                        f = 1.5f / w[i].speed * 128.0f;
                        if (f < (f32)miniBallWork->game.ballHitVol) {
                            miniBallWork->game.ballHitVol = (s32)f;
                            miniBallWork->game.ballHitSe = 1;
                        }
                    }
                }
            }
            w[i].travel = w[i].speed = VuVecNormalizeLen(w[i].vel, vi);
        }
    }
}
