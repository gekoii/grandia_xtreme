#include "common.h"
#include "battle/env.h"
#include "battle/misc.h"

void *EnvParamGet(s32 key) {
    s32 i;
    s32 n;
    s32 *p;

    if (envParamIndex < 0) {
        if (btlFromField == 0) {
            key = 0;
        }
        i = 0;
        n = ENV_PARAM_TABLE->count;
        p = ENV_PARAM_TABLE->mapNos;
        for (; i < n; i++, p++) {
            if (*p == key) {
                break;
            }
        }
        if (!(i < n)) {
            i = 0;
        }
        envParamIndex = i;
        p = &ENV_PARAM_TABLE->mapNos[i];
        if (*p == 0) {
            envContrastAmb = envContrastDir = 0.0f;
        } else {
            envContrastAmb = 0.7f;
            envContrastDir = 0.55f;
        }
    }
    return (u8 *)&ENV_PARAM_TABLE->mapNos[ENV_PARAM_TABLE->count] + envParamIndex * 0x70;
}
