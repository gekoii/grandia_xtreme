#include "common.h"
#include "battle/misc.h"
#include "battle/n31.h"
#include "protos/battle.h"

void MgcFallbackMagicCast(MgcWork *t) {
    if (t->phase == 0x100) {
        MgcSetNextProc(MgcFallbackMagicHit);
        t->phase = 0x114;
    }
}
