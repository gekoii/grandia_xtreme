/* cc: ee-gcc2.9-991111-01 */
#include "common.h"
#include "battle/env.h"

void EnvFlareFade(s32 n, f32 x) {
    if (n <= 0) {
        envFlareAlpha = x;
        envFlareAlphaStep = 0.0f;
        return;
    }
    envFlareAlphaStep = (x > 0.0f ? 1.0f : -1.0f) / (f32)n;
}
