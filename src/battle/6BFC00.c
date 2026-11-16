#include "common.h"
#include "protos/battle.h"
#include "battle/unit.h"

/* btlUnitPos (btlUnits[0].pos) stepped by the unit size: indexing btlUnits[i].pos
 * loads the index before the base (6 words off). */
typedef struct { u8 pad[0x200]; } UnitStride;

void BtlUnitCollectAreaTargetsAtTarget(BtlUnit *arg0) {
    UnitStride *u = (UnitStride *)btlUnitPos;
    s32 i = arg0->target;

    do { /* do-while(0): keeps the call a jal (not a tail call) */
        BtlUnitCollectAreaTargets(arg0, (VuVec3 *)&u[i]);
    } while (0);
}
