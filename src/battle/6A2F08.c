#include "common.h"
#include "protos/battle.h"
#include "sys.h"
#include "vu.h"
#include "battle/n30.h"


void VuMtxSetRotX(void *, s32);
void VuSinCosAngle(f32 *, f32 *, s32);

void FxAntiMagicDraw(void) {
    FVECTOR v;
    FVECTOR w;
    FMATRIX m;
    BtlUnit *u = btlUnits;
    FxAntiMagic *p = fxAntiMagic;
    FxAntiMagicPart *e;
    s32 i;
    s32 j;
    s8 t;
    f32 a;
    f32 g;
    f32 zero;

    a = 0.0f;
    GphTexBind(0x36);
    SprBegin(a, a);
    zero = a;
    sprState.blend = 2;
    for (i = 0; i < 16; i++, u++, p++) {
        if (u->state <= 0) {
            continue;
        }
        if (p->parts == NULL) {
            continue;
        }
        if (u->alpha <= zero) {
            continue;
        }
        e = p->parts;
        sprState.col[0].a = u->alpha * 128.0f;
        a = 0.1f;
        VuMtxSetRotX(m, p->angle);
        VuMtxRotY(m, p->angle, m);
        BtlChrGetPartPos(v, u, 1);
        for (j = 0; j < 30; j++, e++) {
            if (e->life <= 0) {
                continue;
            }
            t = 30 - (u8)e->life;
            if (t < 5) {
                sprState.sx = t * 0.4f;
            } else if (t < 20) {
                sprState.sx = 2.0f - (t - 5) * (1.0f / 15.0f);
            } else {
                sprState.sx = 1.0f - (t - 20) * a;
            }
            sprState.sx = sprState.sy = sprState.sx * e->scale;
            if (t < 10) {
                sprState.col[0].b = 0xFF;
                sprState.col[0].r = 0x40;
                g = t * a;
                sprState.col[0].g = g * 128.0f + zero;
            } else if (t < 20) {
                g = (t - 10) * a;
                sprState.col[0].g = g * -64.0f + 128.0f;
                sprState.col[0].r = g * 64.0f + 64.0f;
                sprState.col[0].b = g * -177.0f + 255.0f;
            } else {
                sprState.col[0].r = 0x80;
                sprState.col[0].g = 0x40;
                g = (t - 20) * a;
                sprState.col[0].b = g * -14.0f + 78.0f;
            }
            sprState.flags = (sprState.flags & 0xFFFFFFFC) | (j & 3);
            VuSinCosAngle(&w[0], &w[2], e->angle);
            w[1] = 0.0f;
            w[0] *= 150.0f;
            w[2] *= 150.0f;
            __asm__ volatile(
                "lqc2 $vf1, 0(%0)\n\t"
                "lqc2 $vf4, 0x0(%1)\n\t"
                "lqc2 $vf5, 0x10(%1)\n\t"
                "lqc2 $vf6, 0x20(%1)\n\t"
                "lqc2 $vf7, 0x30(%1)\n\t"
                "vmulax.xyzw ACC, $vf4, $vf1x\n\t"
                "vmadday.xyzw ACC, $vf5, $vf1y\n\t"
                "vmaddaz.xyzw ACC, $vf6, $vf1z\n\t"
                "vmaddw.xyzw $vf1, $vf7, $vf1w\n\t"
                "sqc2 $vf1, 0(%0)"
                : : "r"(w), "r"(m) : "memory");
            __asm__ volatile(
                "lqc2 $vf1, 0(%0)\n\t"
                "lqc2 $vf2, 0(%1)\n\t"
                "vadd.xyz $vf1, $vf1, $vf2\n\t"
                "sqc2 $vf1, 0(%0)"
                : : "r"(w), "r"(v) : "memory");
            SprDrawRect3D(&ptclGlowRect, -1, -1, w);
        }
    }
    SprFlush();
}
