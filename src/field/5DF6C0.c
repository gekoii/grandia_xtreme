#include "common.h"
#include "protos/field.h"
#include "ptcl.h"
#include "field/state.h"


void EffSpawnUnusedC(s32 arg0) {
    s32 i;

    for (i = 0; i < 4; i++) {
        if (effMapWork->pickupPool[i]->active == 0) {
            PtclPresetVanishConverge(effMapWork->pickupPool[i], arg0);
            return;
        }
    }
    PtclPresetVanishConverge(effMapWork->pickupPool[0], arg0);
}
