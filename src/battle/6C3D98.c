#include "common.h"
#include "protos/battle.h"
#include "party.h"
#include "battle/unit.h"

void VuVecCopy(void *, void *);

void AiCmdEvade(BtlUnit *u) {
    f32 f;

    if (u->params->cmdDisable & 2) {
        VuVecCopy(u->moveTarget, u->pos);
    } else {
        BtlUnitSetFreeSpotTarget(u);
        u->flags |= 0x10;
    }
    f = u->params->restartEvade;
    u->ipMode = 1; //P
    u->ipSpeed = 0; //P
    u->scopeType = 0; //P
    u->ipRet = f; //P
    u->ipPos = f; //P
    u->flags &= 0xFFFFFF7F; //P
    BtlUnitSetState(u, 0x80);
}
