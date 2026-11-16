/* cc: ee-gcc2.9-991111-01 */
#include "common.h"
#include "protos/battle.h"
#include "battle/n29.h"


void FxAfterimageDraw(void) {
    FxAfterimage *p = &fxAfterimage;
    FxAfterimageSnap *q;
    f32 c[4] __attribute__((aligned(16)));
    f32 t;
    f32 u;
    s32 i;
    s32 n;

    if (p->unit != NULL && p->count > 0) {
        for (i = 0; i < p->count; i++) {
            t = (f32)i / (f32)(p->count - 1);
            u = 1.0f - t;
            c[0] = p->color0[0] * u + p->color1[0] * t;
            c[1] = p->color0[1] * u + p->color1[1] * t;
            c[2] = p->color0[2] * u + p->color1[2] * t;
            c[3] = p->color0[3] * u + p->color1[3] * t;
            GphShadowSetColor(c);
            GphDrawListSet(p->drawLists[i]);
            GphShadowDrawLayer(0);
            GphShadowDrawLayer(7);
            n = p->count - 1;
        }
    }
    q = &fxAfterimageSnap;
    if (q->motion != 0) {
        GphShadowSetColor(q);
        GphDrawListSet(fxAfterimageSnapDrawList);
        GphShadowDrawLayer(0);
        GphShadowDrawLayer(7);
    }
}
