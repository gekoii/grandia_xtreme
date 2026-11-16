#include "common.h"
#include "protos/battle.h"
#include "battle/env.h"


void EnvStageBuildDrawList(void) {
    EnvStageZone *z;
    s32 i;

    GphDrawListSet(envStageDrawList);
    GphDrawListClear(NULL);
    GphLightListSet(envLightListGround);
    GphLightListUpdate();
    if (envStageClipOn != 0) {
        z = envStageZones;
        for (i = 0; i < envStageZoneCount; i++, z++) {
            if (EnvStageZoneIsVisible(z) != 0) {
                BtlChrPartsSetHidden(z->part, 1);
            } else {
                BtlChrPartsSetHidden(z->part, 0);
            }
        }
    }
    BtlChrModelCalcDraw(envStageModel);
    GphDrawListSort();
}
