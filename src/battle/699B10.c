/* as: ee-gcc2.9-991111-01 */
#include "common.h"
#include "protos/battle.h"
#include "battle/n29.h"


void FxAtkMultiHitBuild(FxAtk *e) {
    FxAtkMultiHit *o = e->work;
    s32 i;

    for (i = 0; i < o->count; i++) {
        if (!(o->alpha[i] <= 0.0f)) {
            ChrModelSetAlpha(o->models[i], (e->alpha * o->alpha[i] - 1.0f) * 128.0f);
            BtlChrModelCalcDraw(o->models[i]);
        }
    }
}
