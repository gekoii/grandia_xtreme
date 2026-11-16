/* cc: ee-gcc2.9-991111-01 */
/* as: ee-gcc2.9-991111-01 */
#include "common.h"
#include "protos/battle.h"
#include "battle/n33.h"
#include "party.h"
#include "battle/misc.h"

void VuVecCopy(VuVec3 *, VuVec3 *);

#define RAND() ((rand() >> 15) * (1.0f / 65536.0f))
#define SPEED(info) ((info)->params[4] * btlInvPartyAvgSpeed / (btlPartyAliveCount * 60.0f + 240.0f))

void BtlUnitStateWait(BtlUnit *u) {
    BtlUnit *tgt = &btlUnits[u->target];
    PtyUnit *i0;
    PtyUnit *i1;
    PtyUnit *i2;
    s32 k;
    PtyMoveLevel *e;
    PtyMove *sk;
    BtlUnit *o;
    s32 st;
    s32 old;
    s32 v;
    s8 t;
    f32 dx, dz;

    if (u->id < 4 && u->params->angryProb > 0.0f) {
        if (btlActingUnit != u->id) {
            BtlSchedActRequest(u->id);
            return;
        }
        u->action = 4;
        u->skillId = (u->params->id - 0x1001) * 6 + 0x65;
        e = &ptyPartyData->moveLevels[BtlSpecialMoveRecordIndex(u->skillId)];
        u->skillLevel = e->level;
        if (u->skillLevel <= 0) {
            u->skillLevel = 1;
        } else if (e->maxLevel < u->skillLevel) {
            u->skillLevel = e->maxLevel;
        }
        u->combo = 1;
        u->target = u->attacker;
        sk = &ptyMoveTable[u->skillId];
        u->scopeRadius = ((PtyMoveLevelData *)*(s32 *)&sk->levels)[u->skillLevel - 1].scope; /* levels read as s32 */
        BtlUnitSetTargetArea(u, sk->target, sk->unk1);
        BtlUnitFixTarget(u, sk->kind, 1);
        if (u->target == u->id) {
            BtlSchedActDone(u->target);
            return;
        }
        u->params->angryProb = 2.0f;
        BtlUnitActionBegin(u);
        BtlUnitSetState(u, 0xC1);
        u->sealS = 0;
        u->params->status &= 0xFFFFFDFF;
        return;
    }

    if (u->ipPos > 0.0f && btlActingUnit == u->id) {
        if (++u->unk1C5 > 0x3C) {
            BtlSchedActDone(u->id);
        }
    } else {
        u->unk1C5 = 0;
    }

    if (u->id >= 4 && u->ipPos <= 0.5f) {
        st = u->action;
        if (st == 0x7F && (u->phase > 0 || u->subPhase > 0)) {
            AiEnemyChooseCommand(u);
            if (u->action == st) {
                return;
            }
        }
    }

    switch (u->phase) {
    case 0:
        switch (u->subPhase) {
        case 0:
            i0 = u->params;
            u->ipSpeed = SPEED(i0);
            if (!(i0->status & 0x100)) {
                if (!((u->id < 4 ? u->target >= 4 : u->target < 4) && tgt->state >= 0x40)) {
                    BtlUnitSelectTarget(u, 1, 0x1003);
                    tgt = &btlUnits[u->target];
                }
                dx = u->pos[0] - u->moveTarget[0];
                dz = u->pos[2] - u->moveTarget[2];
                if (dx * dx + dz * dz < 100.0f) {
                    k = u->params->id & 0xF0;
                    if (k != 0xC0 && k != 0xD0) {
                        u->action = 0x7F;
                    }
                    VuVecCopy(&u->moveTarget, &u->pos);
                    u->subPhase = 0;
                    u->phase++;
                    break;
                }
            }
            if (u->body < 0) {
                BtlChrSetMotion(u, 3);
            } else {
                u->run = 0;
            }
            u->subPhase++;
        case 1:
            u->run--;
            if (!(u->ipPos <= 0.0f || u->run <= 0 || BtlUnitMoveToward(u, &u->moveTarget, 3, BtlUnitGetMoveSpeed(u), 10.0f) != 0)) {
                break;
            }
            i1 = u->params;
            if ((i1->status & 0x100 || i1->bitCsf & 0x10) && u->ipPos > 0.0f && u->body < 0) {
                u->run = i1->runTime;
                BtlUnitSetFreeSpotTarget(u);
                break;
            }
            if (u->target != u->id && u->id < 4) {
                u->rot[1] = BtlAtan2Angle(tgt->pos[0] - u->pos[0], tgt->pos[2] - u->pos[2]);
            }
            VuVecCopy(&u->moveTarget, &u->pos);
            u->subPhase = 0;
            u->phase++;
            break;
        }
        break;
    case 1:
        if (btlPhase == 0x80) {
            i2 = u->params;
            if ((i2->status & 0x100 || i2->bitCsf & 0x10) && u->ipPos > 0.0f && u->body < 0) {
                u->ipSpeed = SPEED(i2);
                if (u->motionNo != 3) {
                    BtlChrSetMotion(u, 3);
                }
                u->subPhase = 1;
                u->phase = 0;
                break;
            }
        }
        switch (u->subPhase) {
        case 0:
            BtlChrSetMotion(u, 0);
            BtlChrMotionSetFrame(u, (u->motionLast * RAND() > 0.0f) ? (s32)(u->motionLast * RAND()) : -(s32)(-u->motionLast * RAND()));
            u->ipSpeed = SPEED(u->params);
            u->stateCount = 0;
            if (btlPhase != 0x80) {
                u->subPhase = 2;
                break;
            }
            u->subPhase++;
        case 1:
            if (u->body < 0 && u->stateCount < 0x20) {
                BtlUnitMoveDir(u, 0, 3, 0.0f);
            }
            v = u->params->bitCsf;
            if (!(v & 0x20)) {
                if (v & 0x400) {
                    BtlUnitTurnToTarget(u);
                } else {
                    old = u->rot[1];
                    BtlUnitTurnToTarget(u);
                    if (u->rot[1] != old) {
                        if (u->motionNo != 3) {
                            BtlChrSetMotion(u, 3);
                        }
                    } else if (u->motionNo != 0) {
                        BtlChrSetMotion(u, 0);
                    }
                }
            }
            if (u->ipPos <= 0.0f) {
                t = u->comboLeader;
                u->run = u->params->runTime;
                if (t >= 0) {
                    u->action = 6;
                    o = &btlUnits[t];
                    u->target = o->target;
                    u->scopeType = o->scopeType;
                    u->turn++;
                    BtlUnitSetState(u, 0xC0);
                    u->flags |= 0x80;
                    if (btlActingUnit == u->id) {
                        BtlSchedActDone(btlActingUnit);
                    }
                } else if (u->id < 4) {
                    if (btlActingUnit == u->id) {
                        if (AiPartyChooseCommand(u) == 0) {
                            BtlUnitSetState(u, 0x81);
                        } else {
                            BtlSchedActDone(u->id);
                        }
                    } else if (AiPartyChooseCommand(u) == 0) {
                        BtlSchedActRequest(u->id);
                        return;
                    }
                    u->turn++;
                    u->flag &= 0xFF000000;
                } else {
                    BtlEnemyActionStart(u);
                }
                if (u->state != 0x80) {
                    VuVecCopy(&u->moveTarget, &u->pos);
                    u->flags &= 0xFFFFFFEF;
                    u->ipMode = 2;
                    u->ipPos = 1.0f;
                    u->ipSpeed = 0.0f;
                    if (u->id < 4) {
                        if (u->action == 4 && ptyPartyData->moveLevels[BtlSpecialMoveRecordIndex(u->skillId)].level == 0) {
                            u->ipPos = 0.0f;
                        } else if (u->comboLeader >= 0 && ptyPartyData->moveLevels[BtlSpecialMoveRecordIndex((&btlUnits[u->comboLeader])->skillId)].level == 0) {
                            u->ipPos = 0.0f;
                        }
                    }
                }
            }
            break;
        case 2:
            if (btlPhase == 0x80) {
                u->subPhase = 1;
            }
            break;
        }
        break;
    }
}
