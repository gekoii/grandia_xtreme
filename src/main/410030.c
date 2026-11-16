#include "common.h"
#include "protos/main.h"
#include "sys.h"
#include "main/n01.h"

#include "vu.h"

void *memset(void *, s32, u32);
void VuMtxSetRotZ(f32 (*)[4], s32);

#define MULMV(d, m, v) \
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
        : : "r"(d), "r"(m), "r"(v) : "memory")

#define VADD(d, a, b) \
    __asm__ volatile( \
        "lqc2 $vf1, 0(%1)\n\t" \
        "lqc2 $vf2, 0(%2)\n\t" \
        "vadd.xyz $vf1, $vf1, $vf2\n\t" \
        "sqc2 $vf1, 0(%0)" \
        : : "r"(d), "r"(a), "r"(b) : "memory")

void SprDrawRect3D(SprRect *r, s16 x, s16 y, f32 *pos)
{
    GphQuad3D *e;
    SprState *c;
    SprState *c2;
    FVECTOR v[4];
    FMATRIX m;
    s32 i;
    f32 X0;
    f32 Y0;
    f32 X1;
    f32 Y1;

    e = &sprQuad3DList[sprQuad3DCount];
    memset(v, 0, 0x40);
    c = &sprState;
    X0 = (f32)(c->offX + x) * c->sx;
    X1 = (f32)(c->offX + r->w + x) * c->sx;
    Y0 = (f32)(c->offY - y) * c->sy;
    Y1 = (f32)(c->offY - r->h - y) * c->sy;
    v[1][0] = X0;
    v[3][0] = X1;
    v[0][0] = X0;
    v[2][0] = X1;
    v[2][1] = Y0;
    v[0][1] = Y0;
    v[3][1] = Y1;
    v[1][1] = Y1;
    if (c->angle & 0xFFFF) {
        VuMtxSetRotZ(m, -c->angle);
        for (i = 0; i < 4; i++) {
            MULMV(v[i], m, v[i]);
        }
    }
    for (i = 0; i < 4; i++) {
        MULMV(v[i], sprBillboardMatrix, v[i]);
        VADD(&e->pos[i], pos, v[i]);
    }
    e->uv[0][0] = e->uv[1][0] = (sprState.flags & 1) ? r->u1 : r->u0;
    e->uv[2][0] = e->uv[3][0] = (sprState.flags & 1) ? r->u0 : r->u1;
    e->uv[0][1] = e->uv[2][1] = (sprState.flags & 2) ? r->v1 : r->v0;
    e->uv[1][1] = e->uv[3][1] = (sprState.flags & 2) ? r->v0 : r->v1;
    c2 = &sprState;
    e->type = (c2->blend & 3) | 0xC;
    if (c2->flags & 4) {
        e->col[0].r = c2->col[0].r;
        e->col[0].g = c2->col[0].g;
        e->col[0].b = c2->col[0].b;
        e->col[0].a = c2->col[0].a;
        e->col[1].r = c2->col[1].r;
        e->col[1].g = c2->col[1].g;
        e->col[1].b = c2->col[1].b;
        e->col[1].a = c2->col[1].a;
        e->col[2].r = c2->col[2].r;
        e->col[2].g = c2->col[2].g;
        e->col[2].b = c2->col[2].b;
        e->col[2].a = c2->col[2].a;
        e->col[3].r = c2->col[3].r;
        e->col[3].g = c2->col[3].g;
        e->col[3].b = c2->col[3].b;
        e->col[3].a = c2->col[3].a;
    } else {
        e->col[0].r = c2->col[0].r;
        e->col[0].g = c2->col[0].g;
        e->col[0].b = c2->col[0].b;
        e->col[0].a = c2->col[0].a;
        e->col[1].r = c2->col[0].r;
        e->col[1].g = c2->col[0].g;
        e->col[1].b = c2->col[0].b;
        e->col[1].a = c2->col[0].a;
        e->col[2].r = c2->col[0].r;
        e->col[2].g = c2->col[0].g;
        e->col[2].b = c2->col[0].b;
        e->col[2].a = c2->col[0].a;
        e->col[3].r = c2->col[0].r;
        e->col[3].g = c2->col[0].g;
        e->col[3].b = c2->col[0].b;
        e->col[3].a = c2->col[0].a;
    }
    sprQuad3DCount++;
    if (sprQuad3DCount >= 0x20) {
        sprQuad3DList[sprQuad3DCount].type = 0;
        GphBatchDrawQuads3D(sprQuad3DList);
        sprQuad3DCount = 0;
    }
}
