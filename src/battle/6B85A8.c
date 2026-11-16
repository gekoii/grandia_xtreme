/* as: ee-gcc2.9-991111-01 */
#include "common.h"
#include "protos/battle.h"
#include "battle/env.h"


void EnvStageUpdate(void) {
    f32 v;

    ChrModelUvScrollUpdate(envStageModel, 1.0f);
    if (envStageShadeStep != 0.0f) {
        v = envStageShadeAlpha + envStageShadeStep;
        envStageShadeAlpha = v;
        if (envStageShadeStep > 0.0f ? envStageShadeTarget <= v : v <= envStageShadeTarget) {
            envStageShadeStep = 0.0f;
            envStageShadeAlpha = envStageShadeTarget;
        }
    }
}
