#include "common.h"
#include "battle/n29.h"
#include "protos/battle.h"

void FxDelayedCastShadowBuild(FxAtk *e) {
    FxDelayedCast *o = e->work;

    if (e->dead == 0 && o->model != 0) {
        GphDrawListSet(e->drawList);
        GphDrawListClear(0);
        GphModelDrawSimple(o->model);
        ((s32 (*)(void))GphDrawListIndex)(); /* cast: no tail call */
    }
}
