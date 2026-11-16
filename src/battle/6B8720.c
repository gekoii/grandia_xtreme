/* as: ee-gcc2.9-991111-01 */
#include "common.h"
#include "protos/battle.h"
#include "battle/env.h"


void EnvStageShadeDraw(void) {
    if (envStageShadeAlpha > 0.0f) {
        SprBegin(0.0f, 0.0f);
        SprDrawColorQuad(&envStageShadeQuad, envStageShadeAlpha);
        SprFlush();
    }
}
