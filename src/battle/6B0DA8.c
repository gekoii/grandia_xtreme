#include "common.h"
#include "protos/battle.h"
#include "battle/misc.h"
#include "battle/camera.h"
#include "sys.h"


s32 MgcForceEnd(void) {
    MgcWork *d = &mgcWork;

    if (d->state == 0) {
        return 1;
    }
    if (d->state == 3) {
        return 0;
    }
    switch (mgcAbortStep) {
    case 0:
        mgcAbortStep = 1;
        return 0;
    case 1:
        d->model = 0;
        d->update = 0;
        d->draw = 0;
        if ((btlCamParam.flags & 2) && btlCamParam.mode != 1) {
            BtlCamSetMode(1, 0x40, 0, NULL);
        }
        GphScreenFxSet(&btlScreenFx, 0, 1, 0, 0.0f);
        MgcCleanup();
        SndSeStopGroup(2);
        mgcEndFrame = btlFrameCount;
        mgcAbortStep++;
    case 2:
        if (MgcPtclGroupsRelease() == 0) {
            return 0;
        }
        if (FxAfterimageStop() == 0) {
            return 0;
        }
        if (FxCloneIsFree() != 0 && mgcEndFrame != btlFrameCount) {
            d->state = 0;
            mgcAbortStep = 0;
            MemHeapReset(sysHeapBattleMagic[0]);
            return 1;
        }
        return 0;
    default:
        return 0;
    }
}
