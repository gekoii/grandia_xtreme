/* as: ee-gcc2.9-991111-01 */
#include "common.h"
#include "protos/battle.h"
#include "battle/unit.h"


#define RESET(u) { (u)->ipMode = 1; (u)->ipPos = (u)->ipRet; (u)->ipSpeed = 0.0f; (u)->scopeType = 0; (u)->flags &= 0xFFFFFFFE; (u)->flags &= 0xFFFFFF7F; }

void BtlUnitStateActMultiHit(BtlUnit *arg0) {
    BtlUnit *u;
    BtlUnit *t;
    s32 st;
    s32 (*fn)(BtlUnit *);

    if (arg0->partsChr >= 0) {
        u = &btlUnits[arg0->partsChr];
    } else {
        u = arg0;
    }
    t = &btlUnits[u->target];
    st = arg0->phase;
    switch (st) {
    case 0:
        switch (arg0->subPhase) {
        case 0:
            if (BtlAttackApproachStart(arg0, u, t) != 0) {
                break;
            }
            arg0->subPhase++;
        case 1:
            ((s32 (*)(BtlUnit *, BtlUnit *, BtlUnit *))BtlAttackApproach)(arg0, u, t); /* s32 cast of a void callee: v0 handling as in the original */
            break;
        case 2:
            u->flags |= 1;
            BtlChrSetMotionOf(arg0, u, 10);
            arg0->evtLoop = 0;
            arg0->loopStart = arg0->loopEnd;
            BtlUnitLungeInit(arg0);
            arg0->alphaStep = 0.0625f;
            arg0->subPhase++;
        case 3:
            BtlUnitTurnToTarget(arg0);
            BtlUnitLungeStep(arg0);
            if (BtlChrMotionGetEvent(arg0, 1, 0) != 0 || BtlChrMotionGetEvent(arg0, 3, 0) != 0) {
                fn = arg0->def->attack;
                if ((fn == NULL || fn(arg0) == 0) && BtlUnitCheckReach(arg0) != 0) {
                    BtlHitSetNextCallback(BtlAttackOnHit, arg0);
                    BtlHitQueueAddAttack(u, t, BtlAtan2Angle(arg0->pos[0] - t->pos[0], arg0->pos[2] - t->pos[2]), 0);
                }
                arg0->alphaStep = 0.0f;
                arg0->alpha = 1.0f;
                if (--u->combo <= 0) {
                    RESET(u);
                } else {
                    BtlUnitLungeInit(arg0);
                }
                arg0->subPhase = 1;
                arg0->phase++;
            }
            break;
        }
        break;
    case 1:
        if (u->combo > 0) {
            BtlUnitTurnToTarget(arg0);
            BtlUnitLungeStep(arg0);
            if (BtlChrMotionGetEvent(arg0, 1, 0) != 0) {
                if (BtlUnitCheckReach(arg0) != 0) {
                    BtlHitSetNextCallback(BtlAttackOnHit, arg0);
                    BtlHitQueueAddAttack(u, t, BtlAtan2Angle(arg0->pos[0] - t->pos[0], arg0->pos[2] - t->pos[2]), 0);
                }
                if (--u->combo <= 0) {
                    RESET(u);
                } else {
                    BtlUnitLungeInit(arg0);
                }
            }
        }
        if (arg0->subPhase == 0 || arg0->frame == arg0->loopStart) {
            if (u->combo > 0) {
                RESET(u);
            }
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
