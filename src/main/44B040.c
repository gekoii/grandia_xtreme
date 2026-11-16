/* cc: ee-gcc2.9-991111-01 */
/* as: ee-gcc2.96-hazard */
#include "common.h"
#include "protos/main.h"

typedef f32 FVECTOR[4] __attribute__((aligned(16)));
typedef f32 FMATRIX[4][4] __attribute__((aligned(16)));

typedef struct {
    FVECTOR pos;
    f32 f10;
    f32 f14;
    f32 f18;
    f32 f1C;
    FVECTOR v20;
    FVECTOR v30;
} Part;

typedef struct {
    f32 c[8];
    s32 life;
    s32 x24;
    u8 pad28[8];
} PState;

typedef struct {
    u8 v[4][4];
} Col;

typedef struct {
    u8 b[8];
} Tex;

typedef struct {
    f32 f00;
    f32 f04;
    f32 f08;
    u8 pad0C[0x4];
    f32 f10;
    f32 f14;
    f32 f18;
    f32 f1C;
    s32 i20;
    s32 i24;
    s32 i28;
    u8 c2C;
    u8 c2D;
    u8 c2E;
    u8 c2F;
    s32 i30;
    s32 i34;
    s32 i38;
    s32 i3C;
    f32 f40;
    f32 f44;
    f32 f48;
    f32 f4C;
    s16 h50;
    s16 h52;
    f32 f54;
    f32 f58;
    s32 i5C;
    s32 i60;
    s32 i64;
    s32 i68;
    f32 f6C;
    f32 f70;
    s16 h74;
    u8 c76;
    u8 c77;
    u8 c78;
    u8 c79;
    u8 c7A;
    u8 c7B;
    s16 h7C;
    s16 h7E;
    s16 h80;
    s16 h82;
    f32 f84;
    f32 f88;
    f32 f8C;
    f32 f90;
    f32 f94;
    f32 f98;
    f32 f9C;
    f32 fA0;
    f32 fA4;
    f32 fA8;
    f32 fAC;
    f32 fB0;
    f32 fB4;
    f32 fB8;
    f32 fBC;
    f32 fC0;
    f32 fC4;
    f32 fC8;
    f32 fCC;
    f32 fD0;
    s16 hD4;
    s16 hD6;
    f32 fD8;
    s16 hDC;
    s16 hDE;
    f32 fE0;
    s8 bE4[16];
} Key;

typedef struct {
    s16 idx;
    s16 x2;
    s32 flags;
    s32 time;
    s32 cnt;
    f32 accum;
    s16 n;
    s16 pad16;
    Part (*parts)[31];
    Col (*cols)[32];
    s32 x20;
    Tex *tex8;
    PState (*st)[31];
    s32 *counts;
    s32 tex;
    s32 x34;
    s32 x38;
    s32 x3C;
    s32 rx;
    s32 ry;
    s32 rz;
    s32 x4C;
    FVECTOR ofs;
    Key key;
} Emitter;

extern FMATRIX ptclWorkMatrix;

void VuMtxSetRotX(void *, s32);
void VuMtxSetRotY(void *, s32);
void VuVecCopy(f32 *, f32 *);
void VuVecNormalize(f32 *, f32 *);

#define FRAND() ((f32)(rand() >> 15) * (1.0f / 65536.0f))

#define apply(dst, m, src) \
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

#define vadd(dst, a, b) \
    __asm__ volatile( \
        "lqc2 $vf1, 0(%1)\n\t" \
        "lqc2 $vf2, 0(%2)\n\t" \
        "vadd.xyz $vf1, $vf1, $vf2\n\t" \
        "sqc2 $vf1, 0(%0)" \
        : : "r"(dst), "r"(a), "r"(b) : "memory")

#define vscale(dst, src, k) \
    __asm__ volatile( \
        "lqc2 $vf1, 0(%1)\n\t" \
        "qmtc2.ni %2, $vf2\n\t" \
        "vmulx.xyz $vf1, $vf1, $vf2x\n\t" \
        "sqc2 $vf1, 0(%0)" \
        : : "r"(dst), "r"(src), "r"(k) : "memory")

void PtclSpriteEmit(Emitter *e)
{
    FVECTOR tmp;
    Key *k;
    s32 i;
    s32 *counts;
    s32 left;
    s32 j;
    Part *p;
    PState *st;
    Col *c;
    s32 rx;
    s32 ry;
    f32 f;

    k = &e->key;
    if (e->flags & 0x400) {
        return;
    }
    e->accum += k->f54 + k->f58 * FRAND();
    if (!(e->accum >= 1.0f)) {
        return;
    }
    counts = e->counts;
    left = k->h74;
    if (e->tex != 0) {
        GphAnimSetSelect(e->tex);
    }
    for (i = 0; i < e->n; i++, counts++) {
        if (*counts >= 31) {
            continue;
        }
        p = e->parts[i];
        st = e->st[i];
        for (j = 0; j < 31; j++, st++, p++) {
            if (st->life != 0) {
                continue;
            }
            st->life = k->h50 + (s32)(k->h52 * FRAND() + 0.5f);
            if (st->life == 0) {
                continue;
            }
            (*counts)++;
            st->x24 = 0;
            e->x2++;
            if (e->flags & 0x200) {
                rx = k->i5C + k->i60 * e->cnt;
                ry = k->i64 + k->i68 * e->cnt;
            } else {
                rx = k->i5C + (s32)((f32)(k->i60 & 0xFFFFF) * (FRAND() - 0.5f));
                ry = k->i64 + (s32)((f32)(k->i68 & 0xFFFFF) * (FRAND() - 0.5f));
            }
            VuMtxSetRotX(ptclWorkMatrix, rx);
            VuMtxRotY(ptclWorkMatrix, ry, ptclWorkMatrix);
            p->pos[0] = p->pos[2] = 0.0f;
            p->pos[3] = 1.0f;
            f = 1.0f - FRAND();
            f = 1.0f - f * f;
            p->pos[1] = k->f6C + k->f70 * f;
            apply(p->pos, ptclWorkMatrix, p->pos);
            if (e->flags & 0xC) {
                if (e->flags & 4) {
                    rx = k->i30 + (s32)((f32)(k->i34 & 0xFFFFF) * (FRAND() - 0.5f));
                }
                if (e->flags & 8) {
                    ry = k->i38 + (s32)((f32)(k->i3C & 0xFFFFF) * (FRAND() - 0.5f));
                }
                VuMtxSetRotX(ptclWorkMatrix, rx);
                VuMtxRotY(ptclWorkMatrix, ry, ptclWorkMatrix);
            }
            p->v20[0] = p->v20[2] = p->v20[3] = 0.0f;
            p->v20[1] = k->f40 + k->f44 * FRAND();
            apply(p->v20, ptclWorkMatrix, p->v20);
            if (!(e->flags & 2)) {
                if (e->flags & 1) {
                    if (e->flags & 0x800) {
                        VuMtxSetRotX(ptclWorkMatrix, k->i20 + e->rx);
                        VuMtxRotY(ptclWorkMatrix, k->i24 + e->ry, ptclWorkMatrix);
                        VuMtxRotZ(ptclWorkMatrix, k->i28 + e->rz, ptclWorkMatrix);
                    } else {
                        VuMtxSetRotY(ptclWorkMatrix, k->i28 + e->rz);
                        VuMtxRotX(ptclWorkMatrix, k->i20 + e->rx, ptclWorkMatrix);
                        VuMtxRotY(ptclWorkMatrix, k->i24 + e->ry, ptclWorkMatrix);
                    }
                    apply(p->pos, ptclWorkMatrix, p->pos);
                    apply(p->v20, ptclWorkMatrix, p->v20);
                }
                if (e->flags & 0x800) {
                    VuMtxSetRotX(ptclWorkMatrix, e->rx);
                    VuMtxRotY(ptclWorkMatrix, e->ry, ptclWorkMatrix);
                    VuMtxRotZ(ptclWorkMatrix, e->rz, ptclWorkMatrix);
                } else {
                    VuMtxSetRotY(ptclWorkMatrix, e->rz);
                    VuMtxRotX(ptclWorkMatrix, e->rx, ptclWorkMatrix);
                    VuMtxRotY(ptclWorkMatrix, e->ry, ptclWorkMatrix);
                }
                apply(tmp, ptclWorkMatrix, &k->f00);
                vadd(tmp, tmp, e->ofs);
                vadd(p->pos, p->pos, tmp);
            }
            if (e->flags & 0x10) {
                VuVecCopy(p->v30, p->v20);
                if (e->flags & 0x20) {
                    vscale(p->v30, p->v30, -1.0f / (f32)st->life);
                } else {
                    VuVecNormalize(p->v30, p->v30);
                    vscale(p->v30, p->v30, k->f48 + k->f4C * FRAND());
                }
            }
            st->c[0] = k->f8C + k->f90 * FRAND();
            st->c[1] = k->c76 ? k->f94 + k->f98 * FRAND() : st->c[0];
            st->c[2] = k->c77 ? k->f9C + k->fA0 * FRAND() : st->c[1];
            st->c[3] = k->c78 ? k->fA4 + k->fA8 * FRAND() : st->c[2];
            p->f18 = k->f84 + k->f88 * FRAND();
            p->f10 = p->f18 * st->c[0];
            if (e->flags & 0x80) {
                p->f1C = p->f18;
                p->f14 = p->f10;
            } else {
                st->c[4] = k->fB4 + k->fB8 * FRAND();
                st->c[5] = k->c79 ? k->fBC + k->fC0 * FRAND() : st->c[4];
                st->c[6] = k->c7A ? k->fC4 + k->fC8 * FRAND() : st->c[5];
                st->c[7] = k->c7B ? k->fCC + k->fD0 * FRAND() : st->c[6];
                p->f1C = k->fAC + k->fB0 * FRAND();
                p->f14 = p->f1C * st->c[4];
            }
            if ((e->flags & 0x1000) || ((e->flags & 0x2000) && (rand() >> 15) < 0x8000)) {
                p->f18 = -p->f18;
            }
            if ((e->flags & 0x4000) || ((e->flags & 0x8000) && (rand() >> 15) < 0x8000)) {
                p->f1C = -p->f1C;
            }
            if (e->cols != NULL) {
                c = &e->cols[i][j + 1];
                c->v[0][3] = c->v[1][3] = c->v[2][3] = c->v[3][3] = (f32)k->bE4[3];
                if (e->flags & 0x100) {
                    c->v[0][0] = c->v[1][0] = c->v[2][0] = c->v[3][0] = (f32)k->bE4[0];
                    c->v[0][1] = c->v[1][1] = c->v[2][1] = c->v[3][1] = (f32)k->bE4[1];
                    c->v[0][2] = c->v[1][2] = c->v[2][2] = c->v[3][2] = (f32)k->bE4[2];
                }
            }
            if (e->tex != 0) {
                GphAnimStateInit(&e->tex8[i * 31 + j]);
            }
            left--;
            e->cnt++;
            if (left <= 0) {
                left = k->h74;
                e->accum -= 1.0f;
                if (e->accum < 1.0f) {
                    return;
                }
            }
        }
    }
    e->accum = e->accum - (e->accum == (s32)e->accum ? e->accum : (e->accum > 0 ? (s32)e->accum + 1 : ~(s32)-e->accum));
}
