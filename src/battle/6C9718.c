#include "common.h"
#include "protos/battle.h"
#include "battle/n33.h"
#include "battle/camera.h"
#include "party.h"

void VuVecCopy(VuVec3 *, VuVec3 *);

void BtlUnitStateVictoryMember(BtlUnit *u) {
    VuVec3 *p;

    switch (u->phase) {
    case 0:
        switch (u->subPhase) {
        case 0:
            p = &u->moveTarget;
            VuVecCopy(p, &btlUnits[u->target].pos);
            BtlChrSetMotion(u, 3);
            u->alpha = 1.0f;
            FxTraceStopAll(u->id);
            u->stateCount = 0;
            u->subPhase++;
        case 1:
            p = &u->moveTarget;
            if (BtlUnitMoveToward(u, p, 3, BtlUnitGetMoveSpeed(u), 200.0f) != 0 || u->stateCount > 0x80) {
                u->subPhase = 0;
                u->phase++;
            }
            break;
        }
        break;
    case 1:
        switch (u->subPhase) {
        case 0:
            BtlChrSetMotion(u, 0);
            if (u->params->id == 0x1007) {
                u->headLook |= 4;
            } else {
                u->headLook |= 2;
            }
            u->subPhase++;
            u->rot[1] = (btlVictoryCamYaw + 0x6000) & 0xFFFF;
        case 1:
            BtlUnitMoveDir(u, 0, 3, 0.0f);
            break;
        }
        break;
    }
}
