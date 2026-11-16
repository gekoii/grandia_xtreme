#include "common.h"
#include "gph.h"
#include "battle/env.h"
#include "protos/battle.h"

void EnvLightFree(s32 idx) {
    EnvLight *l;

    if (idx >= 3) {
        l = &envLight;
        l->lightFlags[idx] = 0;
        GphLightListSet(l->lists[0]);
        GphLightGet(idx)->enable = 0;
        GphLightListSet(l->lists[1]);
        GphLightGet(idx)->enable = 0;
        GphLightListSet(l->lists[2]);
        GphLightGet(idx)->enable = 0;
    }
}
