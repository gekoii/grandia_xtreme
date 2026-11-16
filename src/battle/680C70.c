#include "common.h"
#include "battle/misc.h"
#include "protos/battle.h"

void BtlModelCalcPass(void) {
    s32 *p = &btlDbgStage;
    *p = 0x40000000;
    BtlChrModelListUpdateGsRegs();
    MgcEndFrame();
    *p = 0;
}
