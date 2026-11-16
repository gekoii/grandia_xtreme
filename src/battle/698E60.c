#include "common.h"
#include "battle/misc.h"
#include "protos/battle.h"

void FxCloneShadowBuild(FxClone *g) {
    s32 i;
    FxCloneEnt *s;

    if (g->unit != 0) {
        s = g->clones;
        for (i = 0; i < g->count;) {
            GphDrawListSet(g->drawLists[i++]);
            GphDrawListClear(0);
            GphModelDrawSimple((s32)s->model);
            s++;
            GphDrawListIndex();
        }
    }
}
