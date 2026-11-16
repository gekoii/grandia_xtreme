#include "common.h"
#include "gph.h"
#include "battle/env.h"
#include "protos/battle.h"

void EnvLightSetRange(s32 idx, f32 v) {
    EnvLight *l;

    if (idx >= 3) {
        l = &envLight;
        GphLightListSet(l->lists[0]);
        GphLightGet(idx)->range = v;
        GphLightListSet(l->lists[1]);
        GphLightGet(idx)->range = v;
        GphLightListSet(l->lists[2]);
        GphLightGet(idx)->range = v;
    }
}
