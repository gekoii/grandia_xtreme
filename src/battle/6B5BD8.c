/* as: ee-gcc2.9-991111-01 */
#include "common.h"
#include "protos/battle.h"
#include "battle/camera.h"
#include "battle/unit.h"
#include "battle/env.h"
#include "party.h"


static inline void copyq(void *dst, void *src) {
    __asm__ volatile("lq $2, 0(%1)\n\tsq $2, 0(%0)" : : "r"(dst), "r"(src) : "$2", "memory");
}

void EnvShadowDraw(void) {
    FVECTOR fog;
    EnvLight *cfg;
    BtlUnit *u;
    s32 *h;
    s32 i;
    f32 d;

    if (btlCamera->eye[1] <= 0.0f) {
        return;
    }
    copyq(fog, envLight.shadowColor);
    cfg = &envLight;
    u = btlUnits;
    h = envShadowDrawLists;
    for (i = 15; i >= 0; h++, i--, u++) {
        if (u->state > 0 && u->model != 0 && u->alpha > 0.0f && !(u->params->status & 1)) {
            d = BtlChrGetCenterY(u) - u->params->colH * 0.5f;
            if (!(d > 500.0f)) {
                if (d < 200.0f) {
                    d = 1.0f;
                } else {
                    d = (500.0f - d) * 0.0033333334f;
                }
                fog[3] = cfg->shadowColor[3] * u->alpha * d;
                GphShadowSetColor(fog);
                GphDrawListSet(*h);
                GphShadowDrawLayer(0);
                GphShadowDrawLayer(7);
            }
        }
    }
    for (i = 0; i < 16; i++) {
        if (envShadowDrawCb[i] != NULL) {
            envShadowDrawCb[i](envShadowCbArg[i]);
        }
    }
}
