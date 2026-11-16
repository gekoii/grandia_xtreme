/* as: ee-gcc2.9-991111-01 */
#include "common.h"
#include "protos/battle.h"
#include "party.h"
#include "battle/n33.h"

static inline f32 fsqrt(f32 x) { f32 r; __asm__ volatile ("sqrt.s %0, %1" : "=f"(r) : "f"(x)); return r; }
void VuSinCosAngle(f32 *, f32 *, s32);
void VuVecCopy(VuVec4 *, VuVec4 *);

void BtlUnitStateEvade(BtlUnit *u) {
    VuVec4 tmp;
    f32 s0, c0;
    f32 s1, c1;
    f32 s2, c2;
    BtlUnit *ta, *tb, *tc, *td, *te, *tf, *tgt, *tg3;
    s32 ang;
    s32 id;
    f32 d;
    f32 dx, dz;
    s32 v;
    s32 i;
    s32 sv88, sv98;
    s8 sv9D;
    s32 sv8C;
    s8 sv9C;
    s32 sv70;
    s32 sw88, sw98;
    s8 sw9D;
    s32 sw8C;
    s8 sw9C;
    s32 sw70;
    f32 sw90;
    f32 sv90;
    s32 type;
    u32 flags;
    f32 f;
    s32 (*cb)(BtlUnit *);

    switch (u->phase) {
    case 0:
        switch (u->subPhase) {
        case 0:
            ta = &btlUnits[u->attacker];
            ang = BtlAtan2Angle(ta->pos[0] - u->pos[0], ta->pos[2] - u->pos[2]);
            u->rot[1] = ang;
            VuSinCosAngle(&s0, &c0, ang + 0x4000);
            {
                f32 r, r2, z, x1, x2, d1;
                r = 100.0f;
                z = c0 * r + u->pos[2];
                x1 = s0 * r + u->pos[0];
                d1 = x1 * x1 + z * z;
                r2 = -100.0f;
                z = c0 * r2 + u->pos[2];
                x2 = s0 * r2 + u->pos[0];
                if (d1 < x2 * x2 + z * z) {
                    u->unk1E4 = 20.0f;
                } else {
                    u->unk1E4 = -20.0f;
                }
            }
            VuVecCopy(&u->moveTarget, &u->pos);
            BtlHudMarkerSpawn(u, 1);
            u->stateCount = 0;
            BtlChrSetMotion(u, 0);
            BtlChrMotionSetFrame(u, BtlChrMotionFindKey(u, 0x407, 0) - 1.0f);
            if (u->params->bitCsf & 0x40000) {
                FxAfterimageSnapshot(u, 255.0f, 255.0f, 255.0f);
                SndSePlay3D(0x7E7, 0, 8, &u->pos);
                u->subPhase = 0;
                u->alpha = 0.0f;
                u->phase++;
                return;
            }
            SndSePlay3D(0x7E6, 0, 8, &u->pos);
            if (u->params->strikeBack > 0.0f &&
                (f32)(rand() >> 15) * (1.0f / 65536.0f) <= u->params->strikeBack) {
                u->flags &= 0xFFFFFFF7;
                BtlChrSetMotion(u, 10);
                u->evtLoop = 0;
                u->loopStart = u->loopEnd;
                id = u->params->id;
                switch (id) {
                case 0x1003:
                case 0x1004:
                case 0x1007:
                case 0x1008:
                    BtlChrMotionSetFrame(u, BtlChrMotionFindKey(u, 4, 0) + 1.0f);
                    BtlChrMotionSetFrame(u, BtlChrMotionFindKey(u, 4, 0));
                    FxTraceStopAll(u->id);
                    if (u->params->id != 0x1007) {
                        FxTraceResume(u);
                    }
                    break;
                }
            }
            u->subPhase++;
        case 1:
            tb = &btlUnits[u->attacker];
            u->rot[1] = BtlAtan2Angle(tb->pos[0] - u->pos[0], tb->pos[2] - u->pos[2]);
            if (u->stateCount < 16) {
                if (u->stateCount < 6 && !(u->stateCount & 1)) {
                    PtclBtlDustSpawn(2, 0, u->pos[0], u->pos[2]);
                }
                VuSinCosAngle(&s1, &c1, u->stateCount << 10);
                BtlUnitMoveDir(u, u->rot[1] + 0x4000, 3, u->unk1E4 * (1.0f - s1));
                if (u->stateCount < 4) {
                    if (u->stateCount == 1) {
                        VuVecCopy(&tmp, &u->pos);
                        VuVecCopy(&u->pos, &u->moveTarget);
                        FxAfterimageSnapshot(u, 0.0f, 50.0f, 128.0f);
                        VuVecCopy(&u->pos, &tmp);
                    }
                }
            }
            if (u->motionNo != 0) {
                if (BtlChrMotionGetEvent(u, 1, 0) != 0 || BtlChrMotionGetEvent(u, 3, 0) != 0) {
                    sv88 = u->action;
                    sv98 = u->skillId;
                    sv9C = u->skillLevel;
                    sv70 = u->target;
                    sv9D = u->ai;
                    sv8C = u->scopeType;
                    sv90 = u->scopeRadius;
                    if (u->id < 4) {
                        u->action = 0;
                        u->skillId = u->def->id - 0xF4C;
                        u->skillLevel = 1;
                    } else {
                        u->action = 0;
                        u->skillId = 0;
                        u->ai = 0;
                    }
                    u->scopeType = (u->attacker < 4) ? 0x901 : 0xA01;
                    u->target = u->attacker;
                    tgt = &btlUnits[u->target];
                    u->scopeRadius = 0.0f;
                    if ((u->def->attack == NULL || u->def->attack(u) == 0) && BtlUnitCheckReach(u) != 0) {
                        BtlHitSetNextFlags(2);
                        BtlHitSetNextCallback((s32)BtlAttackOnHit, (s32)u);
                        BtlHitQueueAddAttack(u, tgt, BtlAtan2Angle(u->pos[0] - tgt->pos[0], u->pos[2] - tgt->pos[2]), 0);
                    }
                    u->skillId = sv98;
                    u->skillLevel = sv9C;
                    u->ai = sv9D;
                    u->scopeType = sv8C;
                    u->scopeRadius = sv90;
                    u->action = sv88;
                    if (sv88 != 0x7F) {
                        u->target = sv70;
                    }
                    u->subPhase++;
                } else if (u->frame == u->loopStart) {
                    u->subPhase++;
                }
            } else if (u->stateCount >= 32) {
                BtlUnitStateResume(u);
            }
            break;
        case 2:
            if (u->frame == u->loopStart || BtlChrMotionGetEvent(u, 5, 0) != 0) {
                BtlUnitStateResume(u);
            }
            break;
        }
        break;
    case 1:
        switch (u->subPhase) {
        case 0:
            if (u->stateCount >= 15) {
                u->subPhase = 1;
            }
            break;
        case 1:
            if (u->params->strikeBack > 0.0f &&
                (f32)(rand() >> 15) * (1.0f / 65536.0f) <= u->params->strikeBack) {
                BtlChrSetMotion(u, (u->id < 4) ? 9 : 10);
                u->loopStart = u->loopEnd;
                tc = &btlUnits[u->attacker];
                u->evtLoop = 0;
                ang = BtlAtan2Angle(tc->pos[0] - u->pos[0], tc->pos[2] - u->pos[2]);
                u->rot[1] = ang;
                VuSinCosAngle(&s2, &c2, ang);
                td = &btlUnits[u->attacker];
                dx = td->pos[0] - u->pos[0];
                dz = td->pos[2] - u->pos[2];
                d = fsqrt(dx * dx + dz * dz) + u->params->colR + td->params->colR;
                if (u->id < 4) {
                    d += (f32)ptyMoveTable[u->def->id - 0xF4C].reach;
                } else {
                    d += (f32)u->def->data->actionSets[u->ai].actions[0]->reach;
                }
                d *= 0.05f;
                s2 *= d;
                c2 *= d;
                for (i = 19; i >= 0; i--) {
                    BtlUnitMoveClamped(u, s2, c2);
                }
            } else {
                BtlUnitSetFreeSpotTarget(u);
                u->pos[0] = u->moveTarget[0];
                u->pos[2] = u->moveTarget[2];
            }
            te = &btlUnits[u->attacker];
            u->rot[1] = BtlAtan2Angle(te->pos[0] - u->pos[0], te->pos[2] - u->pos[2]);
            u->stateCount = 0;
            u->subPhase++;
        case 2:
            if (u->stateCount >= 2) {
                if (u->motionNo != 0) {
                    if (u->motionNo == 9) {
                        f = BtlMotionKeyGetFrame(u->evtCur, 1);
                        if (f <= 0.0f) {
                            f = BtlMotionKeyGetFrame(u->evtCur, 3);
                        }
                        BtlChrMotionSetFrame(u, f - 20.0f);
                    }
                    u->flags &= 0xFFFFFFF7;
                }
                FxAfterimageSnapshot(u, 255.0f, 255.0f, 255.0f);
                u->alphaStep = 0.0625f;
                u->stateCount = 0;
                u->subPhase++;
            }
            break;
        case 3:
            BtlUnitMoveDir(u, 0, 3, 0.0f);
            tf = &btlUnits[u->attacker];
            u->rot[1] = BtlAtan2Angle(tf->pos[0] - u->pos[0], tf->pos[2] - u->pos[2]);
            if (u->motionNo == 0) {
                if (u->alpha >= 1.0f) {
                    BtlUnitStateResume(u);
                }
            } else if (BtlChrMotionGetEvent(u, 1, 0) != 0 || BtlChrMotionGetEvent(u, 3, 0) != 0) {
                sw88 = u->action;
                sw98 = u->skillId;
                sw9C = u->skillLevel;
                sw70 = u->target;
                sw9D = u->ai;
                sw8C = u->scopeType;
                sw90 = u->scopeRadius;
                if (u->id < 4) {
                    u->action = 1;
                    u->skillId = u->def->id - 0xF42;
                    u->skillLevel = 1;
                } else {
                    u->action = 0;
                    u->ai = 0;
                }
                u->target = u->attacker;
                type = (u->attacker < 4) ? 0x900 : 0xA00;
                u->scopeType = type;
                flags = u->params->bitCsf;
                if (flags & 0x20000) {
                    f = 600.0f;
                    if (flags & 0x10000) {
                        f = 2200.0f;
                    }
                    u->scopeRadius = f;
                    u->scopeType = type | 2;
                } else if (flags & 0x10000) {
                    u->scopeType = type | 3;
                    u->scopeRadius = 250.0f;
                } else {
                    u->scopeType = type | 1;
                }
                tg3 = &btlUnits[u->target];
                if (u->def->attack != NULL && u->def->attack(u) != 0) {
                    if ((u8)u->scopeType == 2) {
                    } else if ((u8)u->scopeType == 3) {
                        FxAtkWaveStart(u, 1);
                    }
                } else if (BtlUnitCheckReach(u) != 0) {
                    BtlHitSetNextFlags(2);
                    BtlHitSetNextCallback((s32)BtlAttackOnHit, (s32)u);
                    BtlHitQueueAddAttack(u, tg3, BtlAtan2Angle(u->pos[0] - tg3->pos[0], u->pos[2] - tg3->pos[2]), 0);
                    /* FxAtkRingStart through a cast: f32-before-s32 declaration order (EABI argument load order) */
                    if ((u8)u->scopeType == 2) {
                        ((void (*)(BtlUnit *, s32, BtlUnit *, f32, s32, s32))FxAtkRingStart)(u, 1, tg3, u->scopeRadius, u->scopeType,
                                      (u->id < 4) ? 0 : (s32)BTL_UNIT_ACT(u)->level->effects);
                    } else if ((u8)u->scopeType == 3) {
                        FxAtkWaveStart(u, 1);
                    } else if (u->params->crtclMulti > 0) {
                        if (u->id >= 4 || u->action == 1) {
                            if (u->id < 4) {
                                v = 0;
                            } else {
                                v = (s32)BTL_UNIT_ACT(u)->level->effects;
                            }
                            FxAtkMultiHitStart(u, tg3, v);
                        }
                    }
                } else {
                    if ((u8)u->scopeType == 2) {
                        ((void (*)(BtlUnit *, s32, BtlUnit *, f32, s32, s32))FxAtkRingStart)(u, 0, tg3, u->scopeRadius, u->scopeType,
                                      (u->id < 4) ? 0 : (s32)BTL_UNIT_ACT(u)->level->effects);
                    } else if ((u8)u->scopeType == 3) {
                        FxAtkWaveStart(u, 0);
                    }
                }
                u->skillId = sw98;
                u->skillLevel = sw9C;
                u->ai = sw9D;
                u->scopeType = sw8C;
                u->scopeRadius = sw90;
                u->action = sw88;
                if (sw88 != 0x7F) {
                    u->target = sw70;
                }
                u->subPhase++;
            } else if (u->frame == u->loopStart) {
                u->subPhase++;
            }
            break;
        default:
            if (u->frame == u->loopStart) {
                BtlUnitStateResume(u);
            }
            break;
        }
        break;
    }
}
