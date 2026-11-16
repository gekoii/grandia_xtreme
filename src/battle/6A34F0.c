/* as: ee-gcc2.9-991111-01 */
#include "common.h"
#include "protos/battle.h"
#include "sys.h"
#include "vu.h"
#include "battle/n30.h"


void VuMtxSetRotX(void *, s32);
void VuSinCosAngle(f32 *, f32 *, s32);

void FxStunDraw(void) {
    FVECTOR v;
    FVECTOR w;
    FMATRIX m;
    SprRect rc;
    BtlUnit *u = btlUnits;
    FxStun *p = fxStun;
    SprState *ctx;
    FxStunPart *e;
    PtclCellUv *r;
    s32 i;
    s32 j;
    s8 t;
    f32 g;
    f32 c128;

    GphTexBind(0x38);
    c128 = 128.0f;
    GphAnimSetSelect(ptclAnimSetTex38);
    SprBegin(0.0f, 0.0f);
    sprState.blend = 2;
    for (i = 0; i < 16; i++, u++, p++) {
        if (u->state <= 0) {
            continue;
        }
        if (p->parts == NULL) {
            continue;
        }
        if (u->alpha <= 0.0f) {
            continue;
        }
        e = p->parts;
        VuMtxSetRotX(m, p->angle);
        VuMtxRotY(m, p->angle, m);
        BtlChrGetPartPos(v, u, 1);
        for (j = 0; j < 16; j++, e++) {
            if (e->life <= 0) {
                continue;
            }
            t = 16 - (u8)e->life;
            if (t < 4) {
                sprState.sx = t * 0.050000012f + 1.0f;
            } else if (t < 8) {
                sprState.sx = (t - 4) * 0.07499999f + 1.2f;
            } else {
                sprState.sx = (t - 8) * 0.0625f + 1.5f;
            }
            sprState.sx = sprState.sy = sprState.sx * e->scale * 1.5f;
            if (t < 4) {
                g = t * 0.25f;
                sprState.col[0].r = g * 64.0f + 0.0f;
                sprState.col[0].a = sprState.col[0].g = g * 127.0f + c128;
                sprState.col[0].b = g * -159.0f + 255.0f;
            } else if (t < 8) {
                g = (t - 4) * 0.25f;
                sprState.col[0].g = g * -191.0f + 255.0f;
                sprState.col[0].r = g * 64.0f + 64.0f;
                sprState.col[0].b = g * -32.0f + 96.0f;
                sprState.col[0].a = g * -127.0f + 255.0f;
            } else {
                sprState.col[0].r = 0x80;
                sprState.col[0].g = 0x40;
                sprState.col[0].b = 0x40;
                g = (t - 8) * 0.125f;
                sprState.col[0].a = g * -128.0f + c128;
            }
            ctx = &sprState;
            ctx->flags = (ctx->flags & 0xFFFFFFFC) | (j & 3);
            ctx->col[0].a = ctx->col[0].a * u->alpha;
            VuSinCosAngle(&w[0], &w[2], e->angle);
            w[0] *= 150.0f;
            w[1] = 0.0f;
            w[2] *= 150.0f;
            __asm__ volatile(
                "lqc2 $vf1, 0(%2)\n\t"
                "lqc2 $vf4, 0x0(%1)\n\t"
                "lqc2 $vf5, 0x10(%1)\n\t"
                "lqc2 $vf6, 0x20(%1)\n\t"
                "lqc2 $vf7, 0x30(%1)\n\t"
                "vmulax.xyzw ACC, $vf4, $vf1x\n\t"
                "vmadday.xyzw ACC, $vf5, $vf1y\n\t"
                "vmaddaz.xyzw ACC, $vf6, $vf1z\n\t"
                "vmaddw.xyzw $vf1, $vf7, $vf1w\n\t"
                "sqc2 $vf1, 0(%0)"
                : : "r"(w), "r"(m), "r"(w) : "memory");
            __asm__ volatile(
                "lqc2 $vf1, 0(%1)\n\t"
                "lqc2 $vf2, 0(%2)\n\t"
                "vadd.xyz $vf1, $vf1, $vf2\n\t"
                "sqc2 $vf1, 0(%0)"
                : : "r"(w), "r"(w), "r"(v) : "memory");
            r = (PtclCellUv *)GphAnimGetCellUv(e->anim.cell);
            rc.w = 2;
            rc.h = 2;
            rc.u0 = r->u0 / 8 + 1;
            rc.v0 = r->v0 / 8 + 1;
            rc.u1 = r->u1 / 8 + 1;
            rc.v1 = r->v1 / 8 + 1;
            SprDrawRect3D(&rc, -1, -1, w);
        }
    }
    SprFlush();
}
