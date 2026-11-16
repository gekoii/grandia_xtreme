/* as: ee-gcc2.9-991111-01 */
#include "common.h"
#include "gph.h"
#include "battle/env.h"
#include "protos/battle.h"

void EnvLightSetIntensity(s32 idx, f32 v) {
    EnvLight *l;

    if (idx >= 3) {
        if (v < 0.0f) {
            v = 0.0f;
        }
        l = &envLight;
        GphLightListSet(l->lists[0]);
        GphLightGet(idx)->intensity = v;
        GphLightListSet(l->lists[1]);
        GphLightGet(idx)->intensity = v;
        GphLightListSet(l->lists[2]);
        GphLightGet(idx)->intensity = v;
    }
}
