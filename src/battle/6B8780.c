/* cc: ee-gcc2.9-991111-01 */
#include "common.h"
#include "battle/env.h"

void EnvStageShadeFade(s32 n, f32 x) {
    if (n <= 0) {
        envStageShadeAlpha = x;
        envStageShadeStep = 0.0f;
        return;
    }
    envStageShadeStep = (x - envStageShadeAlpha) / (f32)n;
    envStageShadeTarget = x;
}
