/* as: ee-gcc2.9-991111-01 */
#include "common.h"
#include "protos/field.h"
#include "field/n15.h"

typedef struct {
    s32 hit;
    u8 pad4[0x8];
    f32 r;
    FVECTOR from;
    FVECTOR to;
    FVECTOR out;
} Coll;

extern s32 camFollowClearFlag;
extern Coll camRayQuery;
extern f32 camFollowDistance;

void VuVecCopy(f32 *, f32 *);
f32 VuVecDot(f32 *, f32 *);
void VuVecNormalize(f32 *, f32 *);
f32 VuVecNormalizeLen(f32 *, f32 *);

#define copyq(dst, src) \
    __asm__ volatile("lq $2, 0(%1)\n\tsq $2, 0(%0)" : : "r"(dst), "r"(src) : "$2", "memory")

#define VADD(d, a, b) \
    __asm__ volatile( \
        "lqc2 $vf1, 0(%1)\n\t" \
        "lqc2 $vf2, 0(%2)\n\t" \
        "vadd.xyz $vf1, $vf1, $vf2\n\t" \
        "sqc2 $vf1, 0(%0)" \
        : : "r"(d), "r"(a), "r"(b) : "memory")

#define VSUB(d, a, b) \
    __asm__ volatile( \
        "lqc2 $vf1, 0(%1)\n\t" \
        "lqc2 $vf2, 0(%2)\n\t" \
        "vsub.xyz $vf1, $vf1, $vf2\n\t" \
        "sqc2 $vf1, 0(%0)" \
        : : "r"(d), "r"(a), "r"(b) : "memory")

#define VSCALE(d, a, k) \
    __asm__ volatile( \
        "lqc2 $vf1, 0(%1)\n\t" \
        "qmtc2.ni %2, $vf2\n\t" \
        "vmulx.xyz $vf1, $vf1, $vf2x\n\t" \
        "sqc2 $vf1, 0(%0)" \
        : : "r"(d), "r"(a), "r"(k) : "memory")

static inline f32 fsqrt(f32 x)
{
    f32 r;
    __asm__ volatile("sqrt.s %0, %1" : "=f"(r) : "f"(x));
    return r;
}

void CamFollowClearTurn(f32 *out, f32 *a, f32 *b)
{
    FVECTOR t;
    FVECTOR u;
    FVECTOR w;
    FVECTOR dv;
    FVECTOR dir;
    FVECTOR c;
    Coll *q;
    f32 accel;
    f32 zoom;
    f32 acc2;
    f32 maxv;
    f32 db;
    f32 da;
    f32 spd;
    f32 rad;
    f32 l;
    f32 l2;
    f32 cl;
    f32 m;
    f32 d;
    s32 flag;
    s32 hit;

    accel = camFollowCur->rate[1];
    zoom = camFollowCur->turn;
    acc2 = accel + accel;
    maxv = accel * 20.0f;
    flag = 0;
    copyq(t, camFollowCur->unk90);
    VSUB(c, t, b);
    db = fsqrt(VuVecDot(c, c));
    VSUB(c, t, a);
    da = fsqrt(VuVecDot(c, c));
    VuVecCopy(t, camFollowCur->unk90);
    VSUB(dv, t, b);
    l2 = VuVecNormalizeLen(dir, dv);
    l = camFollowDistance - l2;
    spd = CamApproachSpeed(fabsf(l), camFollowCur->speed, acc2, maxv);
    if (l < 0.0f) {
        spd = -spd;
    }
    camFollowCur->speed = fabsf(spd);
    VSCALE(c, dir, l2 + spd);
    q = &camRayQuery;
    rad = 10.0f;
    {
        f32 *p = c;
        copyq(camRayQuery.to, p);
        copyq(q->out, p);
        copyq(camRayQuery.from, b);
        q->r = rad;
        camRayQuery.hit = 0;
        FldCollSweep((s32 *)q);
    }
    hit = camRayQuery.hit;
    VuVecCopy(out, q->out);
    l = fsqrt(VuVecDot(out, out));
    VuVecNormalize(dir, dv);
    if (hit != 0) {
        l -= 1.0f;
        cl = l;
        VSCALE(dv, dir, l);
        VADD(t, b, dv);
        VSUB(out, t, a);
        l2 = fsqrt(VuVecDot(out, out));
        m = fsqrt(VuVecDot(camFollowCur->unkA0, camFollowCur->unkA0));
        /* do-while(0) left by two breaks (the same exits as gotos are 56 words off) */
        do {
            if (l2 < rad) {
                break;
            }
            flag = 1;
            VSCALE(c, out, 0.5f);
            VADD(u, c, a);
            VSUB(c, u, b);
            {
                f32 *p = c;
                copyq(camRayQuery.to, p);
                copyq(q->out, p);
                copyq(camRayQuery.from, b);
                q->r = rad;
                camRayQuery.hit = 0;
                FldCollSweep((s32 *)q);
            }
            VuVecCopy(c, q->out);
            VADD(w, c, b);
            VSUB(c, w, a);
            l = fsqrt(VuVecDot(out, out));
            VSUB(c, t, a);
            l2 = fsqrt(VuVecDot(c, c));
            VuVecCopy(out, c);
            if (l < da) {
                break;
            }
            VSUB(c, b, a);
            d = fsqrt(VuVecDot(c, c));
            da = fabsf(d - cl) + db;
            l2 = da;
            if (da < rad) {
                l2 = rad;
            }
            spd = CamApproachSpeed(l2, m, accel, maxv + zoom * 100.0f);
            if (da < spd) {
                spd = da;
            }
            VuVecNormalize(out, out);
            VSCALE(out, out, spd);
            VuVecCopy(c, out);
            flag = 2;
            VADD(u, c, a);
            VSUB(c, u, b);
            {
                f32 r = 10.5f;
                f32 *p = c;
                copyq(camRayQuery.to, p);
                copyq(q->out, p);
                copyq(camRayQuery.from, b);
                q->r = r;
                camRayQuery.hit = 0;
                FldCollSweep((s32 *)q);
                VuVecCopy(p, q->out);
                VADD(t, p, b);
            }
            VSUB(out, t, a);
        } while (0);
        camFollowCur->speed = spd;
    } else {
        l -= 1.0f;
        VSCALE(dv, dir, l);
        VADD(t, b, dv);
        VSUB(out, t, a);
        flag = 4;
    }
    camFollowClearFlag = 0;
    camFollowState |= flag;
}
