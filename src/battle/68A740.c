/* as: ee-gcc2.96-hazard */
#include "common.h"
#include "protos/battle.h"
#include "vu.h"
#include "gph.h"
#include "battle/misc.h"
#include "battle/env.h"

typedef struct {
    s16 pad0[2];
    u16 u0;
    u16 v0;
    u16 u1;
    u16 v1;
} TexRect;

extern TexRect ptclBtlDustSprite;

void VuMtxSetRotY(FMATRIX, s32);

#define APPLY(m, v) \
    __asm__ volatile( \
        "lqc2 $vf1, 0(%1)\n\t" \
        "lqc2 $vf4, 0x0(%0)\n\t" \
        "lqc2 $vf5, 0x10(%0)\n\t" \
        "lqc2 $vf6, 0x20(%0)\n\t" \
        "lqc2 $vf7, 0x30(%0)\n\t" \
        "vmulax.xyzw ACC, $vf4, $vf1x\n\t" \
        "vmadday.xyzw ACC, $vf5, $vf1y\n\t" \
        "vmaddaz.xyzw ACC, $vf6, $vf1z\n\t" \
        "vmaddw.xyzw $vf1, $vf7, $vf1w\n\t" \
        "sqc2 $vf1, 0(%1)" \
        : : "r"(m), "r"(v) : "memory")

void PtclBtlDustDrawGround(void)
{
    GphQuad3D poly[2];
    FMATRIX m;
    GphQuad3D *q;
    VuVec4 *v;
    PtclBtlDust *p;
    s32 i;
    s32 j;
    s32 a;
    f32 s;
    f32 ns;

    poly[0].type = 0xD;
    poly[1].type = 0;
    poly[0].col[0].r = 0x18;
    poly[0].col[0].g = 0x15;
    poly[0].col[0].b = 0xF;
    poly[0].col[0].a = 0;
    poly[0].col[1].r = 0x18;
    poly[0].col[1].g = 0x15;
    poly[0].col[1].b = 0xF;
    poly[0].col[1].a = 0;
    poly[0].col[2].r = 0x18;
    poly[0].col[2].g = 0x15;
    poly[0].col[2].b = 0xF;
    poly[0].col[2].a = 0;
    poly[0].col[3].r = 0x18;
    poly[0].col[3].g = 0x15;
    poly[0].col[3].b = 0xF;
    poly[0].col[3].a = 0;
    poly[0].pos[0].y = poly[0].pos[2].y = poly[0].pos[1].y = poly[0].pos[3].y = 0.5f;
    q = poly;
    p = ptclBtlDustPool;
    for (i = 0; i < 128; i++, p++) {
        if (!(p->unk0 <= 0.0f)) {
            a = envLight.shadowColor[3] * 0.6f * p->unk0;
            q->col[0].a = q->col[1].a = q->col[2].a = q->col[3].a = a;
            q->uv[0][0] = q->uv[1][0] = ptclBtlDustSprite.u0;
            q->uv[2][0] = q->uv[3][0] = ptclBtlDustSprite.u1;
            q->uv[0][1] = q->uv[2][1] = ptclBtlDustSprite.v0;
            q->uv[1][1] = q->uv[3][1] = ptclBtlDustSprite.v1;
            s = p->unk4;
            ns = -s;
            q->pos[0].x = q->pos[1].x = q->pos[0].z = q->pos[2].z = ns;
            q->pos[2].x = q->pos[3].x = q->pos[1].z = q->pos[3].z = s;
            VuMtxSetRotY(m, p->unkE);
            for (j = 0; j < 4; j++) {
                v = &q->pos[j];
                APPLY(m, v);
                v->x += p->unk10;
                v->z += p->unk18;
            }
            GphBatchDrawQuads3D(poly);
        }
    }
}
