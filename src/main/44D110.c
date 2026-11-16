/* cc: ee-gcc2.9-991111-01 */
#include "common.h"
#include "ptcl.h"
#include "protos/main.h"

void VuSinCosAngle(f32 *, f32 *, s32);
void VuMtxSetRotX(s32, s32);
void VuMtxSetRotY(s32, s32);

#define VU0_APPLY(dst, m, src) \
    __asm__ volatile( \
        "lqc2 $vf1, 0(%2)\n\t" \
        "lqc2 $vf4, 0x0(%1)\n\t" \
        "lqc2 $vf5, 0x10(%1)\n\t" \
        "lqc2 $vf6, 0x20(%1)\n\t" \
        "lqc2 $vf7, 0x30(%1)\n\t" \
        "vmulax.xyzw ACC, $vf4, $vf1x\n\t" \
        "vmadday.xyzw ACC, $vf5, $vf1y\n\t" \
        "vmaddaz.xyzw ACC, $vf6, $vf1z\n\t" \
        "vmaddw.xyzw $vf1, $vf7, $vf1w\n\t" \
        "sqc2 $vf1, 0(%0)" \
        : : "r"(dst), "r"(m), "r"(src) : "memory")

#define VU0_ADD(dst, a, b) \
    __asm__ volatile( \
        "lqc2 $vf1, 0(%1)\n\t" \
        "lqc2 $vf2, 0(%2)\n\t" \
        "vadd.xyz $vf1, $vf1, $vf2\n\t" \
        "sqc2 $vf1, 0(%0)" \
        : : "r"(dst), "r"(a), "r"(b) : "memory")

void PtclModelMove(PtclGen *e)
{
    FVECTOR a;
    FVECTOR b;
    FVECTOR c;
    FVECTOR d;
    FVECTOR v;
    f32 sc[4];
    s32 mask;
    s32 i;
    PtclGenDef *desc;
    PtclModelPart *p;
    PtclEffector *t;
    GphModel *o;
    f32 k;
    f32 f;
    f32 lo;
    f32 hi;

    mask = 0;
    i = 0;
    desc = &e->def;
    p = e->modelParts;
    a[0] = a[1] = a[2] = a[3] = b[0] = b[1] = b[2] = b[3] =
        c[0] = c[1] = c[2] = c[3] = d[0] = d[1] = d[2] = d[3] = 0.0f;
    do {
        if (e->def.effectors[i] >= 0) {
            t = &ptclCurSystem->effectors[e->def.effectors[i]];
            if (t->flags & 2) {
                if (t->cycle <= 0) {
                    VuMtxSetRotX((s32)ptclWorkMatrix, t->angX + t->angXRange * e->frame);
                    VuMtxRotY((s32)ptclWorkMatrix, t->angY + t->angYRange * e->frame, (s32)ptclWorkMatrix);
                    v[1] = t->scale + t->scaleRange * e->frame;
                } else {
                    VuSinCosAngle(&sc[0], &sc[1], (s32)((f32)e->frame / (f32)t->cycle * 65536.0f));
                    VuMtxSetRotX((s32)ptclWorkMatrix, t->angX + (s32)(t->angXRange * sc[0]));
                    VuMtxRotY((s32)ptclWorkMatrix, t->angY + (s32)(t->angYRange * sc[0]), (s32)ptclWorkMatrix);
                    v[1] = t->scale + t->scaleRange * sc[0];
                }
            } else {
                VuMtxSetRotX((s32)ptclWorkMatrix, t->angX);
                VuMtxRotY((s32)ptclWorkMatrix, t->angY, (s32)ptclWorkMatrix);
                v[1] = t->scale;
            }
            v[0] = v[2] = v[3] = 0.0f;
            VU0_APPLY(v, ptclWorkMatrix, v);
            if (t->flags & 1) {
                mask |= 2;
                if (t->flags & 4) {
                    VU0_ADD(b, b, v);
                } else {
                    VU0_ADD(d, d, v);
                }
            } else {
                mask |= 1;
                if (t->flags & 4) {
                    VU0_ADD(a, a, v);
                } else {
                    VU0_ADD(c, c, v);
                }
            }
        }
        i++;
    } while (i < 4);

    if (mask != 0) {
        if (e->flags & 2) {
            if (e->flags & 0x800) {
                VuMtxSetRotX((s32)ptclWorkMatrix, -(desc->rot[0] + e->rot[0]));
                VuMtxRotZ((s32)ptclWorkMatrix, -(desc->rot[2] + e->rot[2]), (s32)ptclWorkMatrix);
                VuMtxRotY((s32)ptclWorkMatrix, -(desc->rot[1] + e->rot[1]), (s32)ptclWorkMatrix);
            } else {
                VuMtxSetRotY((s32)ptclWorkMatrix, -(desc->rot[1] + e->rot[1]));
                VuMtxRotX((s32)ptclWorkMatrix, -(desc->rot[0] + e->rot[0]), (s32)ptclWorkMatrix);
                VuMtxRotY((s32)ptclWorkMatrix, -(desc->rot[2] + e->rot[2]), (s32)ptclWorkMatrix);
            }
            if (mask & 1) {
                VU0_APPLY(c, ptclWorkMatrix, c);
            }
            if (mask & 2) {
                VU0_APPLY(d, ptclWorkMatrix, d);
            }
        } else if (e->flags & 1) {
            if (e->flags & 0x800) {
                VuMtxSetRotX((s32)ptclWorkMatrix, desc->rot[0] + e->rot[0]);
                VuMtxRotY((s32)ptclWorkMatrix, desc->rot[1] + e->rot[1], (s32)ptclWorkMatrix);
                VuMtxRotZ((s32)ptclWorkMatrix, desc->rot[2] + e->rot[2], (s32)ptclWorkMatrix);
            } else {
                VuMtxSetRotY((s32)ptclWorkMatrix, desc->rot[2] + e->rot[2]);
                VuMtxRotX((s32)ptclWorkMatrix, desc->rot[0] + e->rot[0], (s32)ptclWorkMatrix);
                VuMtxRotY((s32)ptclWorkMatrix, desc->rot[1] + e->rot[1], (s32)ptclWorkMatrix);
            }
            if (mask & 1) {
                VU0_APPLY(a, ptclWorkMatrix, a);
            }
            if (mask & 2) {
                VU0_APPLY(b, ptclWorkMatrix, b);
            }
        }
        if (mask & 1) {
            VU0_ADD(a, a, c);
        }
        if (mask & 2) {
            VU0_ADD(b, b, d);
        }
    }

    for (i = 0; i < e->groupCount; i++, p++) {
        if (p->life != 0) {
            if (p->time >= p->life) {
                p->life = 0;
                e->liveCount--;
            } else {
                o = e->models[i];
                VU0_ADD(o->pos, o->pos, p->vel);
                if (e->flags & 0x10) {
                    VU0_ADD(p->vel, p->vel, p->acc);
                }
                if (mask & 1) {
                    VU0_ADD(o->pos, o->pos, a);
                }
                if (mask & 2) {
                    VU0_ADD(p->vel, p->vel, b);
                }
                k = (f32)p->time / (f32)p->life;
                if (desc->rdxMode != 0) {
                    o->rot[0] += (f32)(p->rotStep[0] - p->rotFrom[0]) * PtclCurveEval(desc->rdxMode, k) + (f32)p->rotFrom[0];
                } else {
                    o->rot[0] += p->rotFrom[0];
                }
                if (desc->rdyMode != 0) {
                    o->rot[1] += (f32)(p->rotStep[1] - p->rotFrom[1]) * PtclCurveEval(desc->rdyMode, k) + (f32)p->rotFrom[1];
                } else {
                    o->rot[1] += p->rotFrom[1];
                }
                if (desc->rdzMode != 0) {
                    o->rot[2] += (f32)(p->rotStep[2] - p->rotFrom[2]) * PtclCurveEval(desc->rdzMode, k) + (f32)p->rotFrom[2];
                } else {
                    o->rot[2] += p->rotFrom[2];
                }
                if (p->time < desc->sxC1) {
                    f = PtclCurveEval(desc->sxCurve[0], (f32)p->time / (f32)desc->sxC1);
                    lo = p->size[0];
                    hi = p->size[1];
                    f = (hi - lo) * f + lo;
                } else if (p->time < desc->sxC2) {
                    f = PtclCurveEval(desc->sxCurve[1], (f32)(p->time - desc->sxC1) / (f32)(desc->sxC2 - desc->sxC1));
                    lo = p->size[1];
                    hi = p->size[2];
                    f = (hi - lo) * f + lo;
                } else {
                    f = PtclCurveEval(desc->sxCurve[2], (f32)(p->time - desc->sxC2) / (f32)(p->life - desc->sxC2));
                    lo = p->size[2];
                    hi = p->size[3];
                    f = (hi - lo) * f + lo;
                }
                o->scale[0] = p->scale * f;
                if (desc->sinXLen > 0) {
                    VuSinCosAngle(&sc[2], &sc[3], (s32)((f32)(p->time + desc->sinXStart) / (f32)desc->sinXLen * 65536.0f));
                    o->scale[0] *= sc[2] * desc->sinXRate + 1.0f;
                }
                o->scale[1] = o->scale[2] = o->scale[0];
                p->time++;
                ChrModelUvScrollUpdate(o, 1.0f);
            }
        }
    }
}
