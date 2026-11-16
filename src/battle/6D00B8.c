/* as: ee-gcc2.9-991111-01 */
#include "common.h"
#include "protos/battle.h"
#include "battle/n33.h"
#include "party.h"

typedef struct {
    f32 x, y, z, w;
} __attribute__((aligned(16))) Vec4;

void VuVecCopy(Vec4 *, VuVec3 *);
void VuVecNormalize(Vec4 *, Vec4 *);

void BtlUnitStateActCharge(BtlUnit *arg0) {
    Vec4 v;
    BtlUnit *u;
    BtlUnit *t;
    f32 d;
    f32 f;

    if (arg0->partsChr >= 0) {
        u = &btlUnits[arg0->partsChr];
    } else {
        u = arg0;
    }
    t = &btlUnits[u->target];
    switch (arg0->phase) {
    case 0:
        switch (arg0->subPhase) {
        case 0:
            arg0->stateCount = 0x100;
            FxTraceStopAll(arg0->id);
            VuVecCopy(&v, &t->pos);
            v.y = 0.0f;
            VuVecNormalize(&v, &v);
            d = -(BtlUnitGetMoveSpeed(u) * 32.0f + t->params->colR);
            if (d < -800.0f) {
                d = -800.0f;
            }
v.x *= d; v.z *= d; arg0->moveTarget[0] = t->pos[0] + v.x; arg0->moveTarget[1] = 0.0f; arg0->moveTarget[2] = t->pos[2] + v.z;
            BtlChrSetMotion(arg0, 3);
            arg0->subPhase++;
        case 1:
            f = BtlUnitGetMoveSpeed(arg0);
            if (BtlUnitMoveToward(arg0, &arg0->moveTarget, 3, f, BtlUnitGetMoveSpeed(arg0) * 68.0f) != 0) {
                arg0->subPhase++;
            } else if (--arg0->run <= 0) {
                FxFailLabelShow(u, 0);
                if (u->id != arg0->id) {
                    BtlUnitSetState(arg0, arg0->rtAct);
                    arg0->phase = arg0->rtP;
                }
            }
            break;
        case 2:
            BtlChrSetMotionOf(arg0, u, 10);
            arg0->loopStart = arg0->loopEnd;
            arg0->alphaStep = 0.0625f;
            arg0->subPhase++;
            arg0->evtLoop = 0;
        case 3:
            f = BtlUnitGetMoveSpeed(u);
            BtlUnitMoveToward(arg0, &arg0->moveTarget, 1, f, BtlUnitGetMoveSpeed(u));
            BtlUnitTurnToTarget(arg0);
            if (BtlChrMotionGetEvent(arg0, 3, 0) != 0) {
                arg0->alphaStep = 0.0f;
                u->flags |= 1;
                arg0->alpha = 1.0f;
                arg0->subPhase++;
            }
            break;
        case 4:
            BtlUnitMoveToward(arg0, &t->pos, 1, BtlUnitGetMoveSpeed(u), t->params->colR);
            if (BtlChrMotionGetEvent(arg0, 1, 0) != 0) {
                u->flags &= 0xFFFFFFFE;
                u->combo--;
                if (BtlUnitCheckReach(arg0) != 0) {
                    BtlHitSetNextCallback(BtlAttackOnHit, arg0);
                    BtlHitQueueAddAttack(u, t, BtlAtan2Angle(arg0->pos[0] - t->pos[0], arg0->pos[2] - t->pos[2]), 0);
                }
u->ipMode = 1; u->ipPos = u->ipRet; u->ipSpeed = 0.0f; u->scopeType = 0; u->flags &= 0xFFFFFF7F; arg0->phase++; arg0->subPhase = 1;
            }
            break;
        }
        break;
    case 1:
        if (arg0->subPhase == 0 || arg0->frame == arg0->loopStart) {
            BtlUnitSetState(u, 0x80);
            u->action = 0x7F;
            BtlUnitSetFreeSpotTarget(arg0);
            if (u->id != arg0->id) {
                BtlUnitSetState(arg0, arg0->rtAct);
                arg0->phase = arg0->rtP;
            }
            arg0->partsChr = -1;
        }
        break;
    }
}
