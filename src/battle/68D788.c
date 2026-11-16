/* cc: ee-gcc2.9-991111-01 */
#include "common.h"
#include "protos/battle.h"
#include "battle/misc.h"

/* GphQuad3D (gph.h) as f32[4] corners and byte colours: with the shared
 * type the function allocates and schedules differently. */
typedef struct {
    s32 type;
    u8 pad4[0xC];
    FVECTOR v[4];
    u8 col[4][4];
    u8 pad60[0x10];
} Prim;

/* Held in one register: byte stores become and/or insertions. */
typedef struct {
    u8 a, b, c, d;
} __attribute__((aligned(4))) Col;

void VuMtxSetRotX(f32 (*)[4], s32);
f32 VuSinAngle(s32);
void VuVecCopy(f32 *, f32 *);

#define APPLY_MATRIX(dst, m, src) \
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

#define ADD_VECTOR(dst, a, b) \
    __asm__ volatile( \
        "lqc2 $vf1, 0(%1)\n\t" \
        "lqc2 $vf2, 0(%2)\n\t" \
        "vadd.xyz $vf1, $vf1, $vf2\n\t" \
        "sqc2 $vf1, 0(%0)" \
        : : "r"(dst), "r"(a), "r"(b) : "memory")

void FxTrailBuildStrip(Prim *prims, FxTrail *t, s32 ang, s16 seg, u16 rot) {
    FMATRIX m;
    Col c;
    f32 f;
    f32 r;
    s32 i;
    s16 k;
    Prim *p;

    f = (f32)seg / (f32)(t->unk1C - 1);
    r = (t->unk2C - t->unk28) * f + t->unk28;
    c.a = ((t->c1.b[0] - t->c0.b[0]) * f + t->c0.b[0]) * *t->unk10 * t->unkC;
    c.d = (t->c1.b[3] - t->c0.b[3]) * f + t->c0.b[3];
    c.c = (t->c1.b[2] - t->c0.b[2]) * f + t->c0.b[2];
    c.b = (t->c1.b[1] - t->c0.b[1]) * f + t->c0.b[1];
    k = (seg & 1) * 2;
    seg = t->unk18 - seg;
    if (seg < 0) {
        seg += t->unk1A;
    }
    VuMtxSetRotX(m, -BtlAcosAngle(t->handle[seg].b[1]));
    VuMtxRotY(m, 0x8000 - ang, m);
    p = prims;
    p->v[k][0] = r * VuSinAngle((0 << 16) / t->unk20 + rot);
    p->v[k][2] = r * VuCosAngle((0 << 16) / t->unk20 + rot);
    p->v[k][1] = 0.0f;
    p->v[k + 1][0] = r * VuSinAngle((-1 << 16) / t->unk20 + rot);
    p->v[k + 1][2] = r * VuCosAngle((-1 << 16) / t->unk20 + rot);
    p->v[k + 1][1] = 0.0f;
    APPLY_MATRIX(p->v[k], m, p->v[k]);
    ADD_VECTOR(p->v[k], p->v[k], t->handle[seg].a);
    APPLY_MATRIX(p->v[k + 1], m, p->v[k + 1]);
    ADD_VECTOR(p->v[k + 1], p->v[k + 1], t->handle[seg].a);
    if (k != 0) {
        p->col[2][0] = p->col[3][0] = c.d;
        p->col[2][1] = p->col[3][1] = c.c;
        p->col[2][2] = p->col[3][2] = c.b;
        p->col[2][3] = p->col[3][3] = c.a;
    } else {
        p->col[0][0] = p->col[1][0] = c.d;
        p->col[0][1] = p->col[1][1] = c.c;
        p->col[0][2] = p->col[1][2] = c.b;
        p->col[0][3] = p->col[1][3] = c.a;
    }
    for (i = 1, p = &prims[1]; i < t->unk20; i++, p++) {
        p->v[k][0] = r * VuSinAngle((i << 16) / t->unk20 + rot);
        p->v[k][2] = r * VuCosAngle((i << 16) / t->unk20 + rot);
        p->v[k][1] = 0.0f;
        APPLY_MATRIX(p->v[k], m, p->v[k]);
        ADD_VECTOR(p->v[k], p->v[k], t->handle[seg].a);
        VuVecCopy(p->v[k + 1], (p - 1)->v[k]);
        if (k != 0) {
            p->col[2][0] = p->col[3][0] = c.d;
            p->col[2][1] = p->col[3][1] = c.c;
            p->col[2][2] = p->col[3][2] = c.b;
            p->col[2][3] = p->col[3][3] = c.a;
        } else {
            p->col[0][0] = p->col[1][0] = c.d;
            p->col[0][1] = p->col[1][1] = c.c;
            p->col[0][2] = p->col[1][2] = c.b;
            p->col[0][3] = p->col[1][3] = c.a;
        }
    }
}
