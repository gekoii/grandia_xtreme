/* as: ee-gcc2.9-991111-01 */
#include "common.h"
#include "battle/camera.h"
#include "battle/env.h"
#include "battle/misc.h"
#include "sys.h"
#include "protos/battle.h"

void EnvFogUpdate(void) {
    EnvParam *e = EnvParamGet(btlEntryMapNo);

    if (!(btlDebugCfg.flags & 0x100)) {
        btlCamera->fogColor[0] = e->fogColor[0] * envDim.cur;
        btlCamera->fogColor[1] = e->fogColor[1] * envDim.cur;
        btlCamera->fogColor[2] = e->fogColor[2] * envDim.cur;
        if (btlCamDistance < 1000.0f) {
            btlCamera->fogDist = e->fogDist;
        } else {
            btlCamera->fogDist = e->fogDist + (btlCamDistance - 1000.0f) * 1.25f;
        }
    }
}
