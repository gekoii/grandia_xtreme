/* cflags: -ffast-math */
/* -ffast-math: sqrtf is a bare sqrt.s (no NaN check and sqrtf fallback call) */
#include "common.h"
#include "protos/battle.h"
#include "party.h"
#include "battle/n33.h"

void VuSinCosAngle(f32 *, f32 *, s32);

void BtlAttackApproach(BtlUnit *arg0, BtlUnit *arg1, BtlUnit *arg2) {
    f32 vx;
    f32 vz;
    f32 dx;
    f32 dz;
    f32 d;
    f32 e;
    f32 r;
    s32 a;
    s32 i;

    if (arg0->stateCount < 16) {
        if (arg0->stateCount >= 14) {
            a = BtlAtan2Angle(arg2->pos[0] - arg0->pos[0], arg2->pos[2] - arg0->pos[2]);
            arg0->rot[1] = a;
            VuSinCosAngle(&vx, &vz, a);
            dx = arg2->pos[0] - arg0->pos[0];
            dz = arg2->pos[2] - arg0->pos[2];
            d = dx * dx + dz * dz;
            d = sqrtf(d);
            do {} while (0); /* do-while(0): block boundary after the sqrt (register choice for arg1/arg2) */
            r = arg2->params->colR;
            e = d - (arg0->params->colR + r);
            e -= (arg0->id < 4) ? ptyMoveTable[arg1->skillId].reach : BTL_UNIT_ACT(arg1)->reach;
            e *= 0.1f;
            vx *= e;
            vz *= e;
            for (i = 9; i >= 0; i--) {
                BtlUnitMoveClamped(arg0, vx, vz);
            }
            FxAfterimageSnapshot(arg0, 255.0f, 255.0f, 255.0f);
            arg0->subPhase++;
        }
    } else if (BtlUnitApproachTarget(arg0) != 0) {
        arg0->subPhase++;
    } else {
        arg1->run--;
        if (arg1->run <= 0) {
            FxFailLabelShow(arg1, 0);
            if (arg1->id != arg0->id) {
                BtlUnitSetState(arg0, arg0->rtAct);
                arg0->phase = arg0->rtP;
            }
        }
    }
}
