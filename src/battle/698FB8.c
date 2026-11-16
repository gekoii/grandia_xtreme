#include "common.h"
#include "protos/battle.h"
#include "battle/n29.h"


void FxAtkLoad(void) {
    fxAtkMultiHitModel = FilePacGetModel(btlSysModelPack, 5);
    fxAtkWaveModel = FilePacGetModel(btlSysModelPack, 6);
    fxAtkRingModel = FilePacGetModel(btlSysModelPack, 7);
}
