#include "common.h"
#include "protos/battle.h"
#include "battle/misc.h"
#include "gph.h"


void FxCloneBuild(void) {
    FxClone *g = &fxClone;
    FxCloneEnt *s;
    GphPart *m;
    s32 i;
    s32 j;
    f32 f;

    if (g->unit != 0) {
        GphDrawListSet(g->drawList);
        GphDrawListClear(0);
        s = g->clones;
        for (i = 0; i < g->count; i++, s++) {
            f = (s->alpha - 1.0f) * 128.0f;
            m = s->model->parts;
            for (j = s->model->partCount; j > 0; j--, m++) {
                if ((m->gsFlags >> 11) & 3) {
                    m->color[0] = f;
                    m->color[1] = f;
                    m->color[2] = f;
                } else {
                    m->color[3] = f;
                }
            }
            BtlChrModelDraw(s->model);
        }
        GphDrawListSort();
        GphDrawListEnvMapCalcUv();
    }
}
