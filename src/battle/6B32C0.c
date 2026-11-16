#include "common.h"
#include "protos/battle.h"
#include "battle/misc.h"
#include "battle/n31.h"


void MgcFallbackMagicStart(MgcWork *t) {
    if (t->phase == 0) {
        BtlCamSetMode(8, 0x78, (s32)t->caster, 0);
        MgcSetNextProc(MgcFallbackMagicCast);
        t->phase = 0x100;
    }
}
