#include "common.h"
#include "protos/battle.h"
#include "battle/misc.h"
#include "battle/unit.h"


void BtlUnitSetFreeSpotTarget(BtlUnit *o) {
    f32 v[4];

    if (btlEncounterFlags <= 0x20) {
        BtlFormationFindSpot(o, v, 1);
        o->moveTarget[0] = v[0];
        o->moveTarget[2] = v[1];
        o->moveTarget[1] = 0.0f;
    }
}
