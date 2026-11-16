/* as: ee-gcc2.9-991111-01 */
#include "common.h"
#include "protos/battle.h"
#include "battle/n29.h"


void FxAtkUpdate(void) {
    FxAtk *p;
    s32 i;

    p = fxAtkPool;
    for (i = 32; i > 0; i--, p++) {
        if (p->flags) {
            if (p->dead) {
                if (p->frameStamp != btlFrameCount) {
                    FxAtkFree(p);
                    continue;
                }
            } else if (p->unit && p->unit->state < 5) {
                p->dead = 1;
                if (p->actRequested) {
                    BtlSchedActCancel(0x10);
                }
                p->frameStamp = btlFrameCount;
                continue;
            }
            if (!(p->flags & 0x100) && btlActingUnit != 0xFF) {
                p->alpha -= 0.0625f;
                if (p->alpha <= 0.0f) {
                    p->alpha = 0.0f;
                }
            } else {
                if (p->alpha < 1.0f) {
                    p->alpha += 0.125f;
                    if (p->alpha > 1.0f) {
                        p->alpha = 1.0f;
                    }
                }
                if (p->update) {
                    p->update(p);
                }
            }
        }
    }
}
