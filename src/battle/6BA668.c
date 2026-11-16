/* as: ee-gcc2.9-991111-01 */
#include "common.h"
#include "protos/battle.h"
#include "battle/misc.h"
#include "battle/unit.h"
#include "party.h"
#include "gph.h"

static inline void copyq(void *dst, void *src) {
    __asm__ volatile("lq $2, 0(%1)\n\tsq $2, 0(%0)" : : "r"(dst), "r"(src) : "$2", "memory");
}


void BtlUnitPreDraw(BtlUnit *u) {
    GphPart *p;
    s32 n;
    s32 i;
    f32 s;
    f32 x;
    f32 y;
    f32 z;

    if (u->alpha > 0.0f && !(u->params->status & 1)) {
        EnvLightSelectForUnit(u);
        if (btlPhase == 0x80) {
            GphDrawListSet(btlFxDrawList);
            FxUnitRegisterModels(u);
            GphDrawListSet(u->id < 4 ? btlPartyDrawList : btlEnemyDrawList);
        }
        if (u->model != NULL) {
            BtlChrModelUpdate(u);
            s = u->alpha;
            x = (u->ambient[0] + 128.0f) * s - 128.0f;
            y = (u->ambient[1] + 128.0f) * s - 128.0f;
            z = (u->ambient[2] + 128.0f) * s - 128.0f;
            n = u->model->partCount;
            p = u->model->parts;
            u->ambient[3] = (s - 1.0f) * 128.0f;
            for (; n > 0; n--, p++) {
                if ((p->gsFlags >> 11) & 3) {
                    p->color[0] = x;
                    p->color[1] = y;
                    p->color[2] = z;
                    p->color[3] = u->ambient[3];
                } else {
                    copyq(p->color, u->ambient);
                }
            }
            for (i = 0; i < 3; i++) {
                if (u->partHooks[i] != NULL) {
                    u->partHooks[i](&btlUnits[u->parts[i]]);
                }
            }
            BtlChrModelDraw(u->model);
        }
    }
}
