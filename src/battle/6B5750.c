/* as: ee-gcc2.9-991111-01 */
#include "common.h"
#include "battle/env.h"

void EnvShadowSetLightBlend(f32 x) {
    if (x < 0.0f) {
        envShadowLightBlend = 0.0f;
    } else if (x >= 1.0f) {
        envShadowLightBlend = 1.0f;
    } else {
        envShadowLightBlend = x;
    }
}
