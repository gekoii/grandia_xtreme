/* as: ee-gcc2.9-991111-01 */
#include "common.h"
#include "protos/battle.h"
#include "vu.h"
#include "sys.h"
#include "battle/n30.h"

void VuVecNormalize(VuVec4 *, VuVec4 *);

#define VSUB(d, a, b) \
    __asm__ volatile( \
        "lqc2 $vf1, 0(%1)\n\t" \
        "lqc2 $vf2, 0(%2)\n\t" \
        "vsub.xyz $vf1, $vf1, $vf2\n\t" \
        "sqc2 $vf1, 0(%0)" \
        : : "r"(d), "r"(a), "r"(b) : "memory")

#define VADD(d, a, b) \
    __asm__ volatile( \
        "lqc2 $vf1, 0(%1)\n\t" \
        "lqc2 $vf2, 0(%2)\n\t" \
        "vadd.xyz $vf1, $vf1, $vf2\n\t" \
        "sqc2 $vf1, 0(%0)" \
        : : "r"(d), "r"(a), "r"(b) : "memory")

#define VSCALE(d, a, s) \
    __asm__ volatile( \
        "lqc2 $vf1, 0(%1)\n\t" \
        "qmtc2.ni %2, $vf2\n\t" \
        "vmulx.xyz $vf1, $vf1, $vf2x\n\t" \
        "sqc2 $vf1, 0(%0)" \
        : : "r"(d), "r"(a), "r"(s) : "memory")

void FxComboLinkDraw(void) {
    FxComboLink *d = &fxComboLink;
    VuVec4 a;
    VuVec4 b;
    FxComboLinkPart *e;
    s32 i;
    s32 t;
    f32 k;

    GphTexBind(0x42);
    SprBegin(0.0f, 0.0f);
    sprState.sx = 4.0f;
    sprState.blend = 2;
    sprState.sy = 4.0f;
    e = fxComboLink.parts;
    BtlChrGetPartPos(&a, d->actor, 1);
    i = 90;
    do {
        if (e->life > 0 && !(d->dist[e->member] < e->radius)) {
            t = 30 - e->life;
            if (t < 5) {
                k = t * 0.2f;
                sprState.col[0].r = 230;
                sprState.col[0].g = 150;
                sprState.col[0].b = 65;
                sprState.col[0].a = k * 192.0f + 0.0f;
            } else if (t < 10) {
                k = (t - 5) * 0.2f;
                sprState.col[0].r = k * -230.0f + 230.0f;
                sprState.col[0].g = k * -60.0f + 150.0f;
                sprState.col[0].b = k * 185.0f + 65.0f;
                sprState.col[0].a = k * -64.0f + 192.0f;
            } else {
                k = (t - 10) * 0.05f;
                sprState.col[0].g = 90;
                sprState.col[0].b = 250;
                sprState.col[0].r = 0;
                sprState.col[0].a = k * -192.0f + 192.0f;
            }
            BtlChrGetPartPos(&b, &btlUnits[e->member], 1);
            VSUB(&b, &b, &a);
            VuVecNormalize(&b, &b);
            VSCALE(&b, &b, e->radius);
            VADD(&b, &b, &a);
            SprDrawRect3D(&ptclGlowRect, -1, -1, &b);
        }
        i--;
        e++;
    } while (i > 0);
    SprFlush();
}
