#include "common.h"
#include "protos/battle.h"
#include "battle/misc.h"
#include "battle/n31.h"


void MgcFallbackNextPart(void *arg0) {
    ((MgcWork *)arg0)->update = MgcFallbackMagicStart;
    MgcSetNextProc(MgcFallbackMagicCast);
    BtlCamSetMode(8, 0x78, (s32)((MgcWork *)arg0)->caster, 0);
}
