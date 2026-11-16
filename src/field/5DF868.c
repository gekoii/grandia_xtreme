#include "common.h"
#include "protos/field.h"
#include "ptcl.h"
#include "field/state.h"


void EffSpawnAtPlayer(s32 arg0, s32 arg1) {
    s32 i;

    for (i = 0; i < 4; i++) {
        if (effMapWork->charaPool[i]->active == 0) {
            PtclPresetBlueSparkle(effMapWork->charaPool[i], arg0);
            return;
        }
    }
    PtclPresetBlueSparkle(effMapWork->charaPool[0], arg0);
}
