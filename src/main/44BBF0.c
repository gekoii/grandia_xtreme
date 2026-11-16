/* cc: ee-gcc2.9-991111-01 */
/* as: ee-gcc2.9-991111-01 */
#include "common.h"
#include "ptcl.h"
#include "protos/main.h"

void VuSinCosAngle(f32 *, f32 *, s32);
void VuMtxSetRotX(void *, s32);
void VuMtxSetRotY(void *, s32);

/* VU0 macros: a = b + c (xyz) with all three addresses as operands, the same
 * with the destination as first source, and v = m * v (4x4 apply). */
#define VU0_ADD3(dst, x, y) \
    __asm__ volatile( \
        "lqc2 $vf1, 0(%1)\n\t" \
        "lqc2 $vf2, 0(%2)\n\t" \
        "vadd.xyz $vf1, $vf1, $vf2\n\t" \
        "sqc2 $vf1, 0(%0)" \
        : : "r"(dst), "r"(x), "r"(y) : "memory")

#define VU0_ADD(dst, src) \
    __asm__ volatile( \
        "lqc2 $vf1, 0(%0)\n\t" \
        "lqc2 $vf2, 0(%1)\n\t" \
        "vadd.xyz $vf1, $vf1, $vf2\n\t" \
        "sqc2 $vf1, 0(%0)" \
        : : "r"(dst), "r"(src) : "memory")

#define VU0_APPLY(v, m) \
    __asm__ volatile( \
        "lqc2 $vf1, 0(%0)\n\t" \
        "lqc2 $vf4, 0x0(%1)\n\t" \
        "lqc2 $vf5, 0x10(%1)\n\t" \
        "lqc2 $vf6, 0x20(%1)\n\t" \
        "lqc2 $vf7, 0x30(%1)\n\t" \
        "vmulax.xyzw ACC, $vf4, $vf1x\n\t" \
        "vmadday.xyzw ACC, $vf5, $vf1y\n\t" \
        "vmaddaz.xyzw ACC, $vf6, $vf1z\n\t" \
        "vmaddw.xyzw $vf1, $vf7, $vf1w\n\t" \
        "sqc2 $vf1, 0(%0)" \
        : : "r"(v), "r"(m) : "memory")

void PtclSpriteMove(PtclGen *e)
{
    FVECTOR a;
    FVECTOR b;
    FVECTOR c;
    FVECTOR d;
    FVECTOR t;
    f32 s0, c0;
    f32 s1, c1;
    f32 s2, c2;
    PtclSpriteKey *k;
    s32 i;
    f32 f;
    s32 fl;
    PtclGenDef *key;
    s32 id;
    f32 v;
    s32 mode;
    PtclSprite *p;
    s32 j;
    PtclEffector *el;
    s32 *cnt;

    mode = 0;
    key = &e->def;
    cnt = e->groupLive;
    a[0] = a[1] = a[2] = a[3] = b[0] = b[1] = b[2] = b[3] =
    c[0] = c[1] = c[2] = c[3] = d[0] = d[1] = d[2] = d[3] = 0.0f;

    for (j = 0; j < 4;) {
        id = key->effectors[j];
        if (id < 0) {
            j++;
            continue;
        }
        el = &ptclCurSystem->effectors[id];
        if (el->flags & 2) {
            if (el->cycle <= 0) {
                VuMtxSetRotX(ptclWorkMatrix, el->angX + el->angXRange * e->frame);
                VuMtxRotY(ptclWorkMatrix, el->angY + el->angYRange * e->frame, ptclWorkMatrix);
                t[1] = el->scale + el->scaleRange * (f32)e->frame;
            } else {
                VuSinCosAngle(&s0, &c0, (s32)((f32)e->frame / (f32)el->cycle * 65536.0f));
                VuMtxSetRotX(ptclWorkMatrix, el->angX + (s32)((f32)el->angXRange * s0));
                VuMtxRotY(ptclWorkMatrix, el->angY + (s32)((f32)el->angYRange * s0), ptclWorkMatrix);
                t[1] = el->scale + el->scaleRange * s0;
            }
        } else {
            VuMtxSetRotX(ptclWorkMatrix, el->angX);
            VuMtxRotY(ptclWorkMatrix, el->angY, ptclWorkMatrix);
            t[1] = el->scale;
        }
        t[0] = t[2] = t[3] = 0.0f;
        VU0_APPLY(t, ptclWorkMatrix);
        fl = el->flags;
        if (fl & 1) {
            mode |= 2;
            if (fl & 4) {
                VU0_ADD3(b, b, t);
            } else {
                VU0_ADD3(d, d, t);
            }
        } else {
            mode |= 1;
            if (fl & 4) {
                VU0_ADD3(a, a, t);
            } else {
                VU0_ADD3(c, c, t);
            }
        }
        j++;
    }

    if (mode != 0) {
        if (e->flags & 2) {
            if (e->flags & 0x800) {
                VuMtxSetRotX(ptclWorkMatrix, -(key->rot[0] + e->rot[0]));
                VuMtxRotZ(ptclWorkMatrix, -(key->rot[2] + e->rot[2]), ptclWorkMatrix);
                VuMtxRotY(ptclWorkMatrix, -(key->rot[1] + e->rot[1]), ptclWorkMatrix);
            } else {
                VuMtxSetRotY(ptclWorkMatrix, -(key->rot[1] + e->rot[1]));
                VuMtxRotX(ptclWorkMatrix, -(key->rot[0] + e->rot[0]), ptclWorkMatrix);
                VuMtxRotY(ptclWorkMatrix, -(key->rot[2] + e->rot[2]), ptclWorkMatrix);
            }
            if (mode & 1) {
                VU0_APPLY(c, ptclWorkMatrix);
            }
            if (mode & 2) {
                VU0_APPLY(d, ptclWorkMatrix);
            }
        } else if (e->flags & 1) {
            if (e->flags & 0x800) {
                VuMtxSetRotX(ptclWorkMatrix, key->rot[0] + e->rot[0]);
                VuMtxRotY(ptclWorkMatrix, key->rot[1] + e->rot[1], ptclWorkMatrix);
                VuMtxRotZ(ptclWorkMatrix, key->rot[2] + e->rot[2], ptclWorkMatrix);
            } else {
                VuMtxSetRotY(ptclWorkMatrix, key->rot[2] + e->rot[2]);
                VuMtxRotX(ptclWorkMatrix, key->rot[0] + e->rot[0], ptclWorkMatrix);
                VuMtxRotY(ptclWorkMatrix, key->rot[1] + e->rot[1], ptclWorkMatrix);
            }
            if (mode & 1) {
                VU0_APPLY(a, ptclWorkMatrix);
            }
            if (mode & 2) {
                VU0_APPLY(b, ptclWorkMatrix);
            }
        }
        if (mode & 1) {
            VU0_ADD(a, c);
        }
        if (mode & 2) {
            VU0_ADD(b, d);
        }
    }

    if (e->animSet != 0) {
        GphAnimSetSelect(e->animSet);
    }

    for (j = 0; j < e->groupCount; j++, cnt++) {
        if (*cnt > 0) {
            p = &e->sprites[j * 31];
            k = &e->keys[j * 31];
            for (i = 0; i < 31; i++, p++, k++) {
                if (k->life == 0) {
                    continue;
                }
                if (k->time >= k->life) {
                    k->life = 0;
                    e->liveCount--;
                    (*cnt)--;
                    p->sizeX = p->sizeY = 0.0f;
                    continue;
                }
                VU0_ADD(p->pos, p->vel);
                if (e->flags & 0x10) {
                    VU0_ADD(p->vel, p->acc);
                }
                if (mode & 1) {
                    VU0_ADD(p->pos, a);
                }
                if (mode & 2) {
                    VU0_ADD(p->vel, b);
                }

                if (k->time < key->sxC1) {
                    f = PtclCurveEval(key->sxCurve[0], (f32)k->time / (f32)key->sxC1);
                    v = (k->kx[1] - k->kx[0]) * f + k->kx[0];
                } else if (k->time < key->sxC2) {
                    f = PtclCurveEval(key->sxCurve[1], (f32)(k->time - key->sxC1) / (f32)(key->sxC2 - key->sxC1));
                    v = (k->kx[2] - k->kx[1]) * f + k->kx[1];
                } else {
                    f = PtclCurveEval(key->sxCurve[2], (f32)(k->time - key->sxC2) / (f32)(k->life - key->sxC2));
                    v = (k->kx[3] - k->kx[2]) * f + k->kx[2];
                }
                p->sizeX = p->baseX * v;
                if (key->sinXLen > 0) {
                    VuSinCosAngle(&s1, &c1, (s32)((f32)(k->time + key->sinXStart) / (f32)key->sinXLen * 65536.0f));
                    p->sizeX = p->sizeX * (s1 * key->sinXRate + 1.0f);
                }

                if (e->flags & 0x80) {
                    v = p->sizeX;
                    if (v < 0.0f) {
                        v = -v;
                    }
                    p->sizeY = v;
                    if (p->baseY < 0.0f) {
                        p->sizeY = -v;
                    }
                } else {
                    if (k->time < key->syC1) {
                        f = PtclCurveEval(key->syCurve[0], (f32)k->time / (f32)key->syC1);
                        v = (k->ky[1] - k->ky[0]) * f + k->ky[0];
                    } else if (k->time < key->syC2) {
                        f = PtclCurveEval(key->syCurve[1], (f32)(k->time - key->syC1) / (f32)(key->syC2 - key->syC1));
                        v = (k->ky[2] - k->ky[1]) * f + k->ky[1];
                    } else {
                        f = PtclCurveEval(key->syCurve[2], (f32)(k->time - key->syC2) / (f32)(k->life - key->syC2));
                        v = (k->ky[3] - k->ky[2]) * f + k->ky[2];
                    }
                    p->sizeY = p->baseY * v;
                    if (key->sinYLen > 0) {
                        VuSinCosAngle(&s2, &c2, (s32)((f32)(k->time + key->sinYStart) / (f32)key->sinYLen * 65536.0f));
                        p->sizeY = p->sizeY * (s2 * key->sinYRate + 1.0f);
                    }
                }

                if (e->animSet != 0) {
                    GphAnimStep(&e->anims[j * 31 + i], 1.0f);
                }
                k->time++;
            }
        }
    }
}
