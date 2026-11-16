#include "common.h"
#include "battle/env.h"

void EnvShadowSetLight(s32 arg0) {
    if (arg0 >= 3) {
        envShadowLightIdx = arg0;
    }
}
