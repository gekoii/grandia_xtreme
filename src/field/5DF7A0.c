#include "common.h"
#include "protos/field.h"
#include "ptcl.h"
#include "field/state.h"


void EffSpawnChara(s32 arg0, u8 arg1) {
    s32 i;

    if (arg1) {
        for (i = 0; i < 4; i++) {
            if (effMapWork->charaPool[i]->active == 0) {
                PtclPresetAppear(effMapWork->charaPool[i], arg0);
                return;
            }
        }
        PtclPresetAppear(effMapWork->charaPool[0], arg0);
    } else {
        for (i = 0; i < 4; i++) {
            if (effMapWork->charaPool[i]->active == 0) {
                PtclPresetVanish(effMapWork->charaPool[i], arg0);
                return;
            }
        }
        PtclPresetVanish(effMapWork->charaPool[0], arg0);
    }
}
