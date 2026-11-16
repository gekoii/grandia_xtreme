#include "common.h"
#include "protos/battle.h"
#include "battle/n33.h"
#include "battle/misc.h"
#include "sys.h"


void BtlUnitStateMgcDie(BtlUnit *u) {
    BtlUnit *t;

    if (u->partsChr >= 0) {
        t = &btlUnits[u->partsChr];
    } else {
        t = u;
    }
    MemHeapSelect(sysState.battleMagicHeap);
    switch (u->phase) {
    case 0:
        SndBgmFadeVolume(btlBgmHandle.b, 0xF0, 0.0f);
        u->subPhase = 0;
        u->phase++;
    case 1:
        if (MgcForceEnd() != 0) {
            u->headLook |= 2;
            t->skillId = 0x5F;
            t->mgcProg = btlMgcTagUnknown;
            MgcStart(0x5F, t, btlMgcTagUnknown);
            u->phase = 0x104;
        }
        break;
    case 0x104:
        break;
    case 0x105:
        MgcSwitchProc();
        t->mgcProg = NULL;
        BtlSchedKoDone(u->id);
        BtlEnemyDrop(u, 0);
        BtlDefeatAddExp(u);
        BtlUnitSetState(u, 0);
        break;
    }
    MemHeapSelect(sysState.battleHeap);
}
