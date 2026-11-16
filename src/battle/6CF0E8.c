/* as: ee-gcc2.9-991111-01 */
#include "common.h"
#include "protos/battle.h"
#include "party.h"
#include "battle/n33.h"


/* The FxAtkRingStart casts: same registers as its definition's (..., s32 flags, f32 radius, s32)
   under EABI; only this order schedules the f12 load before the a3 load. */

void BtlUnitStateActAttack(BtlUnit *arg0) {
    BtlUnit *u;
    BtlUnit *t;
    s32 k;
    s32 n;
    s32 x;
    s32 y;
    s32 (*fn)(BtlUnit *);
    f32 r;
    f32 dx;
    f32 dz;

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
            if (BtlAttackApproachStart(arg0, u, t) != 0) {
                break;
            }
            arg0->subPhase++;
        case 1:
            ((s32 (*)(BtlUnit *, BtlUnit *, BtlUnit *))BtlAttackApproach)(arg0, u, t); /* s32 cast of a void callee: v0 handling as in the original */
            break;
        case 2:
            u->flags |= 1;
            if (u->id < 4 && (k = u->action) == 1) {
                BtlChrSetMotionOf(arg0, u, 9);
                u->combo = k;
            } else {
                k = 5;
                if (arg0->motionNo != 10) {
                    k = 4;
                }
                BtlChrSetMotionOf(arg0, u, 10);
                n = u->params->comboMax + u->def->unk6;
                if (n >= 2 && u->combo != n) {
                    if (u->combo < 2) {
                        BtlChrMotionSetFrame(arg0, BtlChrMotionFindKey(arg0, k, NULL) + 1.0f);
                        BtlChrMotionSetFrame(arg0, BtlChrMotionFindKey(arg0, k, NULL));
                    } else {
                        BtlChrMotionSetFrame(arg0, BtlChrMotionFindKey(arg0, k, NULL));
                    }
                    FxTraceStopAll(arg0->id);
                    if (k == 4 && arg0->params->id != 0x1007) {
                        FxTraceResume(arg0);
                    }
                }
            }
            arg0->evtLoop = 0;
            arg0->loopStart = arg0->loopEnd;
            BtlUnitLungeInit(arg0);
            if (arg0->flags & 0x800) {
                BtlUnitLungeSet(arg0, 0.0f, 0.0f);
                arg0->flags &= 0xFFFFF7FF;
            }
            arg0->alphaStep = 0.0625f;
            arg0->subPhase++;
        case 3:
            BtlUnitTurnToTarget(arg0);
            BtlUnitLungeStep(arg0);
            if (BtlChrMotionGetEvent(arg0, 4, 0) != 0 && arg0->params->id != 0x1007) {
                FxTraceResume(arg0);
            }
            if (BtlChrMotionGetEvent(arg0, 1, 0) != 0 || BtlChrMotionGetEvent(arg0, 3, 0) != 0) {
                fn = arg0->def->attack;
                arg0->alpha = 1.0f;
                u->flags &= 0xFFFFFFFE;
                u->combo--;
                arg0->alphaStep = 0.0f;
                if (fn != NULL && fn(arg0) != 0) {
                    switch ((u8)u->scopeType) {
                    case 2:
                        break;
                    case 3:
                        FxAtkWaveStart(arg0, 1);
                        break;
                    }
                    arg0->flags |= 0x800;
                } else if (BtlUnitCheckReach(arg0) != 0) {
                    BtlHitSetNextCallback(BtlAttackOnHit, arg0);
                    BtlHitQueueAddAttack(u, t, BtlAtan2Angle(arg0->pos[0] - t->pos[0], arg0->pos[2] - t->pos[2]), 0);
                    if ((u8)u->scopeType == 2) {
                        x = 0;
                        if (u->id >= 4) {
                            x = (s32)BTL_UNIT_ACT(u)->level->effects;
                        }
                        ((void (*)(BtlUnit *, s8, BtlUnit *, f32, s32, s32))FxAtkRingStart)(arg0, 1, t, u->scopeRadius, u->scopeType, x);
                    } else if ((u8)u->scopeType == 3) {
                        FxAtkWaveStart(arg0, 1);
                    } else {
                        if (u->params->crtclMulti > 0) {
                            if (u->id >= 4 || u->action == 1) {
                                if (u->id < 4) {
                                    y = 0;
                                } else {
                                    y = (s32)BTL_UNIT_ACT(u)->level->effects;
                                }
                                FxAtkMultiHitStart(arg0, t, y);
                            }
                        }
                    }
                } else {
                    if ((u8)u->scopeType == 2) {
                        x = 0;
                        if (u->id >= 4) {
                            x = (s32)BTL_UNIT_ACT(u)->level->effects;
                        }
                        ((void (*)(BtlUnit *, s8, BtlUnit *, f32, s32, s32))FxAtkRingStart)(arg0, 0, t, u->scopeRadius, u->scopeType, x);
                    } else if ((u8)u->scopeType == 3) {
                        FxAtkWaveStart(arg0, 0);
                    }
                }
                if (u->combo <= 0) {
                    u->ipMode = 1;
                    u->ipPos = u->ipRet;
                    u->ipSpeed = 0.0f;
                    u->scopeType = 0;
                    u->flags &= 0xFFFFFF7F;
                }
                arg0->subPhase = 1;
                arg0->phase++;
            }
            break;
        }
        break;
    case 1:
        if (u->combo <= 0) {
            if (arg0->subPhase == 0 || arg0->frame == arg0->loopStart) {
                BtlUnitSetState(u, 0x80);
                u->action = 0x7F;
                BtlUnitSetFreeSpotTarget(arg0);
                if (u->id != arg0->id) {
                    BtlUnitSetState(arg0, arg0->rtAct);
                    arg0->phase = arg0->rtP;
                }
                arg0->partsChr = -1;
                arg0->flags &= 0xFFFFF7FF;
            }
        } else if (arg0->subPhase == 0 || BtlChrMotionGetEvent(arg0, 5, 0) != 0) {
            arg0->phase = 0;
            if (arg0->flags & 0x800) {
                arg0->subPhase = 2;
            } else {
                r = arg0->params->colR + t->params->colR;
                r += (arg0->id < 4 ? ptyMoveTable[u->skillId].reach : BTL_UNIT_ACT(u)->reach);
                r *= r;
                dx = t->pos[0] - arg0->pos[0];
                dz = t->pos[2] - arg0->pos[2];
                if (dx * dx + dz * dz < r) {
                    arg0->subPhase = 2;
                } else {
                    arg0->subPhase = 0;
                }
            }
        }
        break;
    }
}
