/* as: ee-gcc2.9-991111-01 */
#include "common.h"
#include "protos/battle.h"
#include "battle/n33.h"


void BtlUnitStateActRush(BtlUnit *arg0) {
    BtlUnit *u;
    BtlUnit *t;
    u32 st;
    s32 save;
    BtlAction *act;
    f32 f;

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
            BtlUnitLungeInit(arg0);
            arg0->alphaStep = 0.0625f;
            arg0->subPhase++;
        case 3:
            BtlUnitTurnToTarget(arg0);
            if (arg0->def->type != 3) {
                BtlUnitLungeStep(arg0);
            }
            if (arg0->loopStart > 0.0f) {
                arg0->alphaStep = 0.0f;
                arg0->alpha = 1.0f;
                arg0->stateCount = 100;
                switch (arg0->def->type) {
                case 4:
                    arg0->stateCount = 0x44;
                    break;
                case 3:
                    arg0->stateCount = 0;
                    break;
                default:
                    arg0->stateCount = 0x54;
                    break;
                }
                arg0->subPhase++;
            }
            break;
        case 4:
            if (arg0->def->type == 3) {
                BtlUnitTurnToTarget(arg0);
            }
            f = BtlUnitGetMoveSpeed(u);
            BtlUnitMoveDir(arg0, arg0->rot[1], 3, f + f);
            if (arg0->stateCount >= 2) {
                act = BTL_UNIT_ACT(u);
                save = act->reach;
                act->reach = 0;
                if (BtlUnitCheckReach(arg0) != 0) {
                    BtlHitSetNextCallback(BtlAttackOnHit, arg0);
                    BtlHitQueueAddAttack(u, t, BtlAtan2Angle(arg0->pos[0] - t->pos[0], arg0->pos[2] - t->pos[2]), 0);
                    u->combo--;
                    u->ipMode = 1;
                    u->flags &= 0xFFFFFFFE; u->flags &= 0xFFFFFF7F;
                    u->scopeType = 0;
                    arg0->subPhase = 2;
                    u->ipPos = u->ipRet;
                    arg0->phase++;
                    u->ipSpeed = 0.0f;
                } else if (arg0->stateCount >= 100) {
                    BtlHudMarkerSpawn(u, 2);
u->combo--; u->ipMode = 1; u->flags &= 0xFFFFFFFE; u->flags &= 0xFFFFFF7F; u->scopeType = 0; u->ipSpeed = 0.0f; u->ipPos = u->ipRet;
                    arg0->phase++;
                    arg0->subPhase = 2;
                    BtlChrSetMotionOf(arg0, u, 10);
                    BtlChrMotionSetFrame(arg0, BtlMotionKeyGetFrame(arg0->evtCur, 0x408) + 0.5f);
                    arg0->evtLoop = 0;
                    arg0->loopStart = arg0->loopEnd;
                }
                BTL_UNIT_ACT(u)->reach = save;
            }
            break;
        }
        break;
    case 1:
        if (arg0->def->type == 4 || arg0->subPhase == 2) {
            f = BtlUnitGetMoveSpeed(arg0);
            BtlUnitMoveDir(arg0, arg0->rot[1], 3, f + f);
            if (arg0->subPhase == 2) {
                if (arg0->stateCount >= 100 || (arg0->def->type != 4 && t->state != 0x44)) {
                    arg0->subPhase = 1;
                    BtlChrSetMotionOf(arg0, u, 10);
                    BtlChrMotionSetFrame(arg0, BtlMotionKeyGetFrame(arg0->evtCur, 0x408) + 0.5f);
                    arg0->evtLoop = 0;
                    arg0->loopStart = arg0->loopEnd;
                }
                break;
            }
        }
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
