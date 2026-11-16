/* as: ee-gcc2.9-991111-01 */
#include "common.h"
#include "battle/env.h"
#include "protos/battle.h"

void EnvLightSelectDim(s32 on) {
    if (on == 0 || envDim.cur >= 1.0f) {
        if (envLightListSel == 0) {
            return;
        }
        GphLightListSet(envLightListChar);
        envLightListSel = 0;
    } else {
        if (envLightListSel == 1) {
            return;
        }
        GphLightListSet(envLightListCharDim);
        envLightListSel = 1;
    }
    GphLightListUpdate();
}
