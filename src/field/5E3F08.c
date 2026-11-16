/* cc: ee-gcc2.9-991111-01 */
/* as: ee-gcc2.96-hazard */
#include "common.h"

typedef f32 FVECTOR[4] __attribute__((aligned(16)));

typedef struct {
    f32 *p0;
    f32 *p1;
    f32 dx;
    f32 dz;
    f32 len;
} Wall;

typedef struct {
    s32 flag;
    u8 pad4[0xC - 0x4];
    f32 radius;
    FVECTOR pos;
    FVECTOR vel;
    FVECTOR out1;
    FVECTOR out2;
} Mover;

void VuVecCopy(f32 *, f32 *);
f32 VuVecDot(f32 *, f32 *);
s32 MapBorderSweepCircle(f32 *, Mover *, f32);

#define VSUB(d, a, b) __asm__ volatile( \
    "lqc2 $vf1, 0(%1)\n\t" \
    "lqc2 $vf2, 0(%2)\n\t" \
    "vsub.xyz $vf1, $vf1, $vf2\n\t" \
    "sqc2 $vf1, 0(%0)" \
    : : "r"(d), "r"(a), "r"(b) : "memory")

#define VADD(d, a, b) __asm__ volatile( \
    "lqc2 $vf1, 0(%1)\n\t" \
    "lqc2 $vf2, 0(%2)\n\t" \
    "vadd.xyz $vf1, $vf1, $vf2\n\t" \
    "sqc2 $vf1, 0(%0)" \
    : : "r"(d), "r"(a), "r"(b) : "memory")

#define VSCALE(d, a, k) __asm__ volatile( \
    "lqc2 $vf1, 0(%1)\n\t" \
    "qmtc2.ni %2, $vf2\n\t" \
    "vmulx.xyz $vf1, $vf1, $vf2x\n\t" \
    "sqc2 $vf1, 0(%0)" \
    : : "r"(d), "r"(a), "r"(k) : "memory")

s32 MapBorderSweepWall(Wall *w, Mover *m)
{
    FVECTOR hit;
    FVECTOR p;
    FVECTOR v;
    FVECTOR dir;
    FVECTOR nrm;
    FVECTOR r1;
    FVECTOR r2;
    FVECTOR rel;
    FVECTOR end;
    FVECTOR step;
    f32 radius;
    f32 d;
    f32 dn;
    f32 k;
    f32 t;
    f32 len;
    f32 e;

    dir[0] = w->dx;
    dir[1] = 0.0f;
    dir[2] = w->dz;
    nrm[0] = -w->dz;
    nrm[1] = 0.0f;
    nrm[2] = w->dx;
    radius = m->radius;
    { f32 *_s = w->p0; p[0] = *_s++; p[1] = *_s++; p[2] = *_s; }
    { f32 *_s = m->pos; rel[0] = _s[0]; rel[1] = _s[1]; rel[2] = _s[2]; }
    VSUB(rel, rel, p);
    VuVecCopy(v, m->vel);
    d = VuVecDot(rel, nrm);
    if (d < 0.0f) {
        VSCALE(nrm, nrm, -1.0f);
        d = -d;
    }
    VADD(end, v, rel);
    if (radius < VuVecDot(end, nrm)) {
        return 0;
    }
    dn = -VuVecDot(v, nrm);
    if (dn < 0.0f) {
        return 0;
    }
    if (dn < 0.01f) {
        k = 0.0f;
    } else if (radius < d) {
        k = (d - radius) / dn;
    } else {
        k = 0.0f;
    }
    VSCALE(step, v, k);
    VADD(r1, step, rel);
    t = VuVecDot(dir, r1);
    len = w->len;
    if (len < t) {
        VuVecCopy(hit, dir);
        VSCALE(hit, hit, len);
        VADD(hit, hit, p);
        goto call; /* goto: one shared call (a second return adds an insn) */
    }
    if (t < 0.0f) {
        VuVecCopy(hit, p);
call:
        return MapBorderSweepCircle(hit, m, 0.0f);
    }
    e = d - radius;
    t = -VuVecDot(nrm, v);
    if (e > 0.0f) {
        if (dn < 0.001f) {
            e = 0.0f;
        } else {
            e = e / t;
        }
        VSCALE(r1, v, e);
        e = 1.0f - e;
        VSCALE(r2, v, e);
        k = VuVecDot(dir, r2);
        VSCALE(r2, dir, k);
        VuVecCopy(m->out1, r1);
        VuVecCopy(m->out2, r2);
    } else {
        k = VuVecDot(v, dir);
        VSCALE(r2, dir, k);
        e = __builtin_fabsf(e);
        VSCALE(r1, nrm, e);
        VuVecCopy(m->out1, r1);
        VuVecCopy(m->out2, r2);
    }
    m->flag = 1;
    return 1;
}
