#include "common.h"
#include "protos/battle.h"
#include "battle/n30.h"


void FxUnitUpdate(BtlUnit *arg0) {
    if (fxCastAura[arg0->id].kind > 0) {
        FxCastAuraUpdate(arg0);
    }
    FxStatusIconsUpdate(arg0);
    FxFailLabelUpdate(arg0);
    if (fxWall[arg0->id].state > 0) {
        FxWallUpdate(arg0);
    }
    if (fxWallShards[arg0->id].kind > 0) {
        FxWallShardsUpdate(arg0);
    }
    FxRegenUpdate(arg0);
    FxAntiMagicUpdate(arg0);
    FxStunUpdate(arg0);
    FxBindUpdate(arg0);
    FxDefeatBurstUpdate(arg0);
    FxWeaponAuraUpdate(arg0);
}
