#include "common.h"
#include "protos/field.h"
#include "ptcl.h"
#include "field/state.h"


void EffSpawnPickup(s32 arg0) {
    s32 i;

    for (i = 0; i < 4; i++) {
        if (effMapWork->pickupPool[i]->active == 0) {
            PtclPresetItemSparkle(effMapWork->pickupPool[i], arg0);
            return;
        }
    }
    PtclPresetItemSparkle(effMapWork->pickupPool[0], arg0);
}
