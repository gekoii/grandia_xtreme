#include "common.h"
#include "protos/battle.h"
#include "battle/misc.h"
#include "battle/n28.h"
#include "grxsys.h"
/* The FilePacGetModel calls go through a cast: called directly (as in
 * 698FB8.c), the %hi of the stored globals is hoisted into s-registers. */
void FxHitLoad(void) {
    fxHitFlashModel = ((void *(*)(s32, u32))FilePacGetModel)(btlSysModelPack, 0);
    fxHitFlashMotion = (struct GphMotion *)sysGrxsysPac->chunkEC;
    fxHitModel = ((void *(*)(s32, u32))FilePacGetModel)(btlSysModelPack, 2);
    fxHitMotion = (struct GphMotion *)sysGrxsysPac->chunkF0;
    fxHitSkillModel = ((void *(*)(s32, u32))FilePacGetModel)(btlSysModelPack, 8);
    fxHitCounterModel = ((void *(*)(s32, u32))FilePacGetModel)(btlSysModelPack, 0xB);
    fxHitSkillMotion = (struct GphMotion *)sysGrxsysPac->chunk138;
    fxHitCounterMotion = (struct GphMotion *)sysGrxsysPac->chunk13C;
}
