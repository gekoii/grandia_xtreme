#include "common.h"
#include "battle/env.h"
#include "protos/battle.h"

void EnvStageDraw(void) {
    GphDrawListSet(envStageDrawList);
    GphDrawListDrawAll();
    if (envStageZoneDisp != 0) {
        EnvStageZoneDebugDraw();
    }
}
