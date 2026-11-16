/* as: ee-gcc2.9-991111-01 */
#include "common.h"
#include "protos/field.h"
#include "field/n15.h"


s32 CamFollowTrailMove(f32 *, f32 *, f32 *);
s32 CamRayTest(f32 *, f32 *, f32 *);
void VuVecCopy(f32 *, f32 *);
f32 VuVecDot(f32 *, f32 *);
void VuVecNormalize(f32 *, f32 *);
f32 VuVecNormalizeLen(f32 *, f32 *);

#define VSUB(d, a, b) \
    __asm__ volatile( \
        "lqc2 $vf1, 0(%1)\n\t" \
        "lqc2 $vf2, 0(%2)\n\t" \
        "vsub.xyz $vf1, $vf1, $vf2\n\t" \
        "sqc2 $vf1, 0(%0)" \
        : : "r"(d), "r"(a), "r"(b) : "memory")

#define VSUBW(d, a, b) \
    __asm__ volatile( \
        "lqc2 $vf1, 0(%1)\n\t" \
        "lqc2 $vf2, 0(%2)\n\t" \
        "vsub.xyzw $vf1, $vf1, $vf2\n\t" \
        "sqc2 $vf1, 0(%0)" \
        : : "r"(d), "r"(a), "r"(b) : "memory")

#define VSCALE(d, a, k) \
    __asm__ volatile( \
        "lqc2 $vf1, 0(%1)\n\t" \
        "qmtc2.ni %2, $vf2\n\t" \
        "vmulx.xyz $vf1, $vf1, $vf2x\n\t" \
        "sqc2 $vf1, 0(%0)" \
        : : "r"(d), "r"(a), "r"(k) : "memory")

#define VOUTER(d, a, b) \
    __asm__ volatile( \
        "lqc2 $vf1, 0(%1)\n\t" \
        "lqc2 $vf2, 0(%2)\n\t" \
        "vopmula.xyz ACC, $vf1, $vf2\n\t" \
        "vopmsub.xyz $vf1, $vf2, $vf1\n\t" \
        "sqc2 $vf1, 0(%0)" \
        : : "r"(d), "r"(a), "r"(b) : "memory")

#define VUNIT(d) __asm__ volatile("sqc2 $vf0, 0(%0)" : : "r"(d) : "memory")

static inline f32 fsqrt(f32 x)
{
    f32 r;
    __asm__ volatile("sqrt.s %0, %1" : "=f"(r) : "f"(x));
    return r;
}

void CamFollowBlockedTurn(f32 *out, f32 *a, f32 *b)
{
    FVECTOR up;
    FVECTOR dir;
    FVECTOR d;
    FVECTOR pos;
    FVECTOR v;
    f32 h;
    f32 len;
    f32 dot;
    s32 flag;

    CamTrailGet(CamTrailCount(), pos);
    h = camFollowCur->turn;
    VSUB(d, a, pos);
    len = fsqrt(VuVecDot(d, d));
    VSUB(dir, b, a);
    VUNIT(up);
    up[1] = 1.0f;
    VOUTER(out, up, dir);
    VSUBW(d, a, pos);
    if (h < 0.0f) {
        VSCALE(out, out, -1.0f);
    }
    CamFollowTrailMove(d, a, b);
    VuVecNormalizeLen(out, out);
    dot = VuVecDot(out, d);
    if (dot > 0.0f) {
        flag = 0;
        h = fabsf(h) * len;
        if (dot < h) {
            VSCALE(out, out, h);
        } else {
            VuVecCopy(out, d);
        }
        CamRayTest(out, a, pos);
    } else {
        f32 dist;
        f32 cur;
        f32 spd;
        flag = 1;
        VSUB(d, b, a);
        dist = fsqrt(VuVecDot(d, d));
        cur = VuVecNormalizeLen(v, camFollowCur->unkA0);
        VuVecDot(out, v);
        spd = CamApproachSpeed(dist, cur, 0.5f, 20.0f);
        VSUB(out, pos, a);
        VuVecNormalize(out, out);
        VSCALE(out, out, spd);
    }
    camFollowState |= flag;
}
