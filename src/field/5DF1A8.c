#include "common.h"
#include "protos/field.h"
#include "gph.h"

void EffSpawnByCode(GphPart *, s32, s32);

void EffSpawnModelEffects(GphModel *model) {
    s32 i;
    GphPart *e;

    if (model != NULL) {
        for (i = 0; i < model->partCount; i++) {
            e = ChrModelPartAt(model, i);
            if (e->effect != 0) {
                EffSpawnByCode(e, e->effect, (s32)e->mtx);
            }
        }
    }
}
