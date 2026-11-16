/* cc: ee-gcc2.9-991111-01 */
#include "common.h"
#include "protos/battle.h"
#include "battle/n30.h"


void FxWallShardsRegister(BtlUnit *arg0) {
    FxWallShards *p = &fxWallShards[arg0->id];
    FxWallShard *e;
    s32 i;
    f32 f;

    if (p->state == 2) {
        e = p->shards;
        i = 20;
        do {
            if (e->life > 0) {
                if (e->age < 30) {
                    f = 1.0f;
                } else {
                    f = (f32)(e->life - e->age) / (f32)(e->life - 30);
                }
                ChrModelSetAlpha(e->model, (f * arg0->alpha - 1.0f) * 128.0f);
                BtlChrModelCalcDraw(e->model);
            }
            i--;
            e++;
        } while (i > 0);
    }
}
