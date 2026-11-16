#include "common.h"
#include "battle/env.h"

s8 EnvShadowAddCallback(EnvShadowCb a, EnvShadowCb cb, s32 arg) {
    s32 i;

    for (i = 0; i < 0x10; i++) {
        if (envShadowDrawCb[i] == NULL) {
            envShadowPreCb[i] = a;
            envShadowDrawCb[i] = cb;
            envShadowCbArg[i] = arg;
            return i;
        }
    }
    return -1;
}
