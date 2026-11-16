#include "common.h"
#include "protos/battle.h"
#include "battle/n33.h"
#include "battle/misc.h"
#include "battle/env.h"
#include "battle/camera.h"
#include "party.h"
#include "sys.h"


s32 BtlVictoryCheck(void) {
    EnvParam *e;
    BtlUnit *u;
    PtyUnit *o;
    s32 i;
    u32 mask;
    s32 k82, k83;

    if (btlPhase != 0x80) {
        goto fail; /* gotos to the shared exits at the end: early returns change the layout */
    }
    if (btlEnemyAliveCount > 0) {
        goto err;
    }
    e = EnvParamGet(btlEntryMapNo);
    btlVictoryCamYaw = ((rand() >> 15) & 1 | 0x8000) - e->flareAngleY;
    if (btlVictoryLeader >= 4 || btlUnits[btlVictoryLeader].state < 0x40) {
        u = btlUnits;
        for (i = 0; i < 4; i++, u++) {
            if (u->state >= 0x40) {
                break;
            }
        }
        btlVictoryLeader = i;
    }
    u = btlUnits;
    i = 0;
    mask = 0xFFFF9814;
    k82 = 0x82;
    k83 = 0x83;
    for (; i < 4; i++, u++) {
        if (u->state < 0x40) {
            continue;
        }
        o = u->params;
        u->regene = 0;
        u->antiMagic = 0;
        u->transp = 0;
        u->vanish = 0;
        u->confu = 0;
        u->sealM = 0;
        u->sealS = 0;
        u->bind = 0;
        u->sleep = 0;
        o->status &= mask;
        PtyUnitCalcParams(o, 0, 0);
        u->ipSpeed = 0;
        if (i != btlVictoryLeader) {
            if (u->state >= 0x80) {
                u->evtLoop = 0;
                u->evtCur = 0;
                BtlUnitSetState(u, 0x82);
            } else {
                u->retState = k82;
                u->retPhase = 0;
            }
            u->target = btlVictoryLeader;
        } else {
            if (u->state >= 0x80) {
                BtlUnitSetState(u, 0x83);
            } else {
                u->retState = k83;
                u->retPhase = 0;
            }
        }
    }
    btlPhase = 0x81;
    SndBgmFadeVolume(btlBgmHandle.b, 0xF0, 0.0f);
    IpGaugeSetShown(0);
    return 1;
err:
    btlSpecialUsedMask = 0;
fail:
    return 0;
}
