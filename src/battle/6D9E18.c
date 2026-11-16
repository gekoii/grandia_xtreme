#include "common.h"
#include "protos/battle.h"
#include "battle/unit.h"

void VuVecCopy(void *, void *);

void BtlUnitStateResume(BtlUnit *u) {
    BtlHudComboPopupAdd(u->id);
    BtlUnitSetState(u, u->retState);
    u->phase = u->retPhase;
    VuVecCopy(u->moveTarget, u->pos);
    u->subPhase = 0;
    u->flags &= ~8u;
    u->run -= 0x14;
}
