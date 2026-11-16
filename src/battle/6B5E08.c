#include "common.h"
#include "battle/env.h"

void EnvShadowRemoveCallback(s8 i) {
    if ((u8)i < 0x10) {
        EnvShadowCb *pa = &envShadowPreCb[i];
        EnvShadowCb *pb = &envShadowDrawCb[i];
        s32 *pc = &envShadowCbArg[i];
        *pb = NULL;
        *pc = 0;
        *pa = NULL;
    }
}
