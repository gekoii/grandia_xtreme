#include "common.h"
#include "protos/battle.h"
#include "battle/n30.h"


void FxUnitRegisterModels(BtlUnit *arg0) {
    if (fxCastAura[arg0->id].kind > 0) {
        FxCastAuraRegister(arg0);
    }
    if (fxWall[arg0->id].state > 0) {
        FxWallRegister(arg0);
    }
    if (fxWallShards[arg0->id].kind > 0) {
        FxWallShardsRegister(arg0);
    }
}
