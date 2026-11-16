/* as: ee-gcc2.9-991111-01 */
#include "common.h"
#include "protos/battle.h"
#include "party.h"
#include "battle/n33.h"


s32 BtlAttackApproachStart(BtlUnit *arg0, BtlUnit *arg1, BtlUnit *arg2) {
    f32 r;
    f32 dx;
    f32 dz;

    arg0->stateCount = 0x100;
    FxTraceStopAll(arg0->id);
    r = arg0->params->colR + arg2->params->colR;
    arg0->flags &= 0xFFFFF7FF;
    r += (arg0->id < 4) ? ptyMoveTable[arg1->skillId].reach : BTL_UNIT_ACT(arg1)->reach;
    if (arg0->def->type != 3) {
        dx = arg0->pos[0] - arg2->pos[0];
        dz = arg0->pos[2] - arg2->pos[2];
        if (r * r < dx * dx + dz * dz) {
            BtlChrSetMotion(arg0, 3);
            if (arg1->params->bitCsf & 0x80000) {
                FxAfterimageSnapshot(arg0, 255.0f, 255.0f, 255.0f);
                SndSePlay3D(0x7E7, 0, 8, &arg0->pos[0]);
                arg0->alpha = 0;
                arg0->stateCount = 0;
            }
            goto ret0; /* goto: keeps the return-0 block after the return-1 block */
        }
    }
    BtlUnitTurnToTarget(arg0);
    arg0->subPhase = 2;
    return 1;
ret0:
    return 0;
}
