/* cc: ee-gcc2.9-991111-01 */
/* as: ee-gcc2.96-hazard */
#include "common.h"
#include "protos/main.h"

typedef f32 FVECTOR[4] __attribute__((aligned(16)));
typedef f32 FMATRIX[4][4] __attribute__((aligned(16)));

typedef struct {
    u8 pad0[0x10];
    FVECTOR pos;
    s32 unk20;
    s32 unk24;
    s32 unk28;
    s32 pad2C;
    f32 unk30;
} Sprite;

typedef struct {
    s32 life;
    s32 unk4;
    s32 a[3];
    s32 b[3];
    f32 unk20;
    u8 pad24[0xC];
    FVECTOR vel;
    FVECTOR acc;
    f32 c[4];
} Particle;

typedef struct {
    FVECTOR pos;
    u8 pad10[0x10];
    s32 rot[3];
    s32 pad2C;
    s32 unk30;
    s32 unk34;
    s32 unk38;
    s32 unk3C;
    f32 unk40;
    f32 unk44;
    f32 unk48;
    f32 unk4C;
    s16 unk50;
    s16 unk52;
    f32 unk54;
    f32 unk58;
    s32 unk5C;
    s32 unk60;
    s32 unk64;
    s32 unk68;
    f32 unk6C;
    f32 unk70;
    s16 unk74;
    u8 unk76[3];
    u8 pad79[0x84 - 0x79];
    f32 unk84;
    f32 unk88;
    f32 c[4][2];
    u8 padAC[0xFB - 0xAC];
    s8 unkFB[3];
    u8 padFE[2];
    s32 r[9][2];
} Param;

typedef struct {
    s16 unk0;
    u16 unk2;
    s32 flags;
    s32 unk8;
    s32 unkC;
    f32 accum;
    s16 n;
    u8 pad16[0x38 - 0x16];
    Sprite **list;
    Particle *parts;
    s32 rot[3];
    s32 pad4C;
    FVECTOR pos;
    Param p;
} Emitter;

extern FMATRIX ptclWorkMatrix;

void VuMtxSetRotX(FMATRIX, s32);
void VuMtxSetRotY(FMATRIX, s32);
void VuVecCopy(f32 *, f32 *);
void VuVecNormalize(f32 *, f32 *);

#define RAND() ((f32)(rand() >> 15) * (1.0f / 65536.0f))
#define CEIL(x) ((x) == (s32)(x) ? (x) : ((x) > 0 ? (s32)(x) + 1 : ~(s32)(-x)))

#define APPLY(dst, m, src) \
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

#define ADD(dst, a, b) \
    __asm__ volatile( \
        "lqc2 $vf1, 0(%1)\n\t" \
        "lqc2 $vf2, 0(%2)\n\t" \
        "vadd.xyz $vf1, $vf1, $vf2\n\t" \
        "sqc2 $vf1, 0(%0)" \
        : : "r"(dst), "r"(a), "r"(b) : "memory")

#define SCALE(dst, src, s) \
    __asm__ volatile( \
        "lqc2 $vf1, 0(%1)\n\t" \
        "qmtc2.ni %2, $vf2\n\t" \
        "vmulx.xyz $vf1, $vf1, $vf2x\n\t" \
        "sqc2 $vf1, 0(%0)" \
        : : "r"(dst), "r"(src), "r"(s) : "memory")

void PtclModelEmit(Emitter *e)
{
    FVECTOR tmp;
    Param *p;
    Particle *parts;
    Sprite *s;
    f32 *vel;
    f32 *pos;
    s32 i;
    s32 cnt;
    s32 rx;
    s32 ry;
    f32 t;

    p = &e->p;
    if (e->flags & 0x400) {
        return;
    }
    e->accum += p->unk54 + p->unk58 * RAND();
    if (e->accum >= 1.0f) {
        cnt = p->unk74;
        parts = e->parts;
        for (i = 0; i < e->n; i++, parts++) {
            if (parts->life != 0) {
                continue;
            }
            parts->life = p->unk50 + (s32)((f32)p->unk52 * RAND() + 0.5f);
            if (parts->life == 0) {
                continue;
            }
            parts->unk4 = 0;
            s = e->list[i];
            e->unk2++;
            if (e->flags & 0x200) {
                rx = p->unk5C + p->unk60 * e->unkC;
                ry = p->unk64 + p->unk68 * e->unkC;
            } else {
                rx = p->unk5C + (s32)((f32)(p->unk60 & 0xFFFFF) * (RAND() - 0.5f));
                ry = p->unk64 + (s32)((f32)(p->unk68 & 0xFFFFF) * (RAND() - 0.5f));
            }
            pos = s->pos;
            VuMtxSetRotX(ptclWorkMatrix, rx);
            VuMtxRotY(ptclWorkMatrix, ry, ptclWorkMatrix);
            s->pos[0] = s->pos[2] = 0.0f;
            s->pos[3] = 1.0f;
            t = 1.0f - RAND();
            t = 1.0f - t * t;
            s->pos[1] = p->unk6C + p->unk70 * t;
            APPLY(pos, ptclWorkMatrix, pos);
            if (e->flags & 0xC) {
                if (e->flags & 4) {
                    rx = p->unk30 + (s32)((f32)(p->unk34 & 0xFFFFF) * (RAND() - 0.5f));
                }
                if (e->flags & 8) {
                    ry = p->unk38 + (s32)((f32)(p->unk3C & 0xFFFFF) * (RAND() - 0.5f));
                }
                VuMtxSetRotX(ptclWorkMatrix, rx);
                VuMtxRotY(ptclWorkMatrix, ry, ptclWorkMatrix);
            }
            parts->vel[0] = parts->vel[2] = parts->vel[3] = 0.0f;
            parts->vel[1] = p->unk40 + p->unk44 * RAND();
            vel = parts->vel;
            APPLY(vel, ptclWorkMatrix, parts->vel);
            if (!(e->flags & 2)) {
                if (e->flags & 1) {
                    if (e->flags & 0x800) {
                        VuMtxSetRotX(ptclWorkMatrix, p->rot[0] + e->rot[0]);
                        VuMtxRotY(ptclWorkMatrix, p->rot[1] + e->rot[1], ptclWorkMatrix);
                        VuMtxRotZ(ptclWorkMatrix, p->rot[2] + e->rot[2], ptclWorkMatrix);
                    } else {
                        VuMtxSetRotY(ptclWorkMatrix, p->rot[2] + e->rot[2]);
                        VuMtxRotX(ptclWorkMatrix, p->rot[0] + e->rot[0], ptclWorkMatrix);
                        VuMtxRotY(ptclWorkMatrix, p->rot[1] + e->rot[1], ptclWorkMatrix);
                    }
                    APPLY(pos, ptclWorkMatrix, pos);
                    APPLY(vel, ptclWorkMatrix, vel);
                }
                if (e->flags & 0x800) {
                    VuMtxSetRotX(ptclWorkMatrix, e->rot[0]);
                    VuMtxRotY(ptclWorkMatrix, e->rot[1], ptclWorkMatrix);
                    VuMtxRotZ(ptclWorkMatrix, e->rot[2], ptclWorkMatrix);
                } else {
                    VuMtxSetRotY(ptclWorkMatrix, e->rot[2]);
                    VuMtxRotX(ptclWorkMatrix, e->rot[0], ptclWorkMatrix);
                    VuMtxRotY(ptclWorkMatrix, e->rot[1], ptclWorkMatrix);
                }
                APPLY(tmp, ptclWorkMatrix, p->pos);
                ADD(tmp, tmp, e->pos);
                ADD(pos, pos, tmp);
            }
            if (e->flags & 0x10) {
                VuVecCopy(parts->acc, vel);
                if (e->flags & 0x20) {
                    SCALE(parts->acc, parts->acc, -1.0f / (f32)parts->life);
                } else {
                    VuVecNormalize(parts->acc, parts->acc);
                    SCALE(parts->acc, parts->acc, p->unk48 + p->unk4C * RAND());
                }
            }
            s->unk20 = (f32)p->r[0][0] + (f32)p->r[0][1] * RAND();
            s->unk24 = (f32)p->r[1][0] + (f32)p->r[1][1] * RAND();
            s->unk28 = (f32)p->r[2][0] + (f32)p->r[2][1] * RAND();
            parts->a[0] = (f32)p->r[3][0] + (f32)p->r[3][1] * RAND();
            parts->b[0] = p->unkFB[0] == 0 ? parts->a[0] : (f32)p->r[4][0] + (f32)p->r[4][1] * RAND();
            parts->a[1] = (f32)p->r[5][0] + (f32)p->r[5][1] * RAND();
            parts->b[1] = p->unkFB[1] == 0 ? parts->a[1] : (f32)p->r[6][0] + (f32)p->r[6][1] * RAND();
            parts->a[2] = (f32)p->r[7][0] + (f32)p->r[7][1] * RAND();
            parts->b[2] = p->unkFB[2] == 0 ? parts->a[2] : (f32)p->r[8][0] + (f32)p->r[8][1] * RAND();
            parts->c[0] = p->c[0][0] + p->c[0][1] * RAND();
            parts->c[1] = p->unk76[0] != 0 ? p->c[1][0] + p->c[1][1] * RAND() : parts->c[0];
            parts->c[2] = p->unk76[1] != 0 ? p->c[2][0] + p->c[2][1] * RAND() : parts->c[1];
            parts->c[3] = p->unk76[2] != 0 ? p->c[3][0] + p->c[3][1] * RAND() : parts->c[2];
            cnt--;
            parts->unk20 = p->unk84 + p->unk88 * RAND();
            s->unk30 = parts->unk20 * parts->c[0];
            ChrModelUvScrollReset(s);
            e->unkC++;
            if (cnt <= 0) {
                cnt = p->unk74;
                e->accum -= 1.0f;
                if (e->accum < 1.0f) {
                    return;
                }
            }
        }
        e->accum = e->accum - CEIL(e->accum);
    }
}
