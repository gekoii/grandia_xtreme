/* as: ee-gcc2.9-991111-01 */
#include "common.h"
#include "battle/env.h"

void EnvFlareUpdate(void) {
    if (envFlareAlphaStep > 0.0f) {
        envFlareAlpha += envFlareAlphaStep;
        if (envFlareAlpha > 1.0f) {
            envFlareAlpha = 1.0f;
            envFlareAlphaStep = 0.0f;
        }
    } else if (envFlareAlphaStep < 0.0f) {
        envFlareAlpha += envFlareAlphaStep;
        if (envFlareAlpha < 0.0f) {
            envFlareAlpha = 0.0f;
            envFlareAlphaStep = 0.0f;
        }
    }
}
