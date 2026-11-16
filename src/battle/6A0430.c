#include "common.h"
#include "protos/battle.h"
#include "battle/misc.h"
#include "battle/n30.h"
#include "grxsys.h"


void FxUnitLoad(void) {
    fxCastAuraModelMove = FilePacGetModel(btlSysModelPack, 3);
    fxCastAuraModelMagic = FilePacGetModel(btlSysModelPack, 4);
    fxCastAuraBurstMotion = (GphMotion *)sysGrxsysPac->chunkF4;
    fxWallModel = FilePacGetModel(btlSysModelPack, 9);
    fxWallShardModel = FilePacGetModel(btlSysModelPack, 10);
}
