/* as: ee-gcc2.9-991111-01 */
#include "common.h"
#include "protos/battle.h"
#include "vu.h"
#include "sys.h"
#include "party.h"
#include "battle/unit.h"
#include "battle/camera.h"
#include "battle/misc.h"
#include "battle/n33.h"

void VuVecCopy(f32 *, f32 *);

/* Access casts below: headLook as s8 and scopeType stored as f32 (0.0f), as this
 * function's old local view had them; the header types change the allocation. */
#define RNDBIT() ((rand() >> 15) & 1)

void BtlUnitStateActMgc(BtlUnit *arg0) {
    BtlUnit *t;
    BtlUnit *m;
    BtlUnit *o;
    s32 j;
    BtlCharDef *a;
    PtyUnit *info;
    s16 *s;
    s32 i;

    if (arg0->partsChr >= 0) {
        t = &btlUnits[arg0->partsChr];
    } else {
        t = arg0;
    }
    MemHeapSelect(sysState.battleMagicHeap);
    switch (arg0->phase) {
    case 0:
        BtlUnitTurnToTarget(arg0);
        if (arg0->subPhase == 0) {
            if (arg0->parts[0] >= 0 && arg0->motionNo != 0) {
                BtlChrSetMotion(arg0, 0);
            }
            arg0->subPhase++;
            if (t->mgcProg != 0 && t->id < 4
                && (ptyPartyData->moveLevels[BtlSpecialMoveRecordIndex(t->skillId)].level == 0 || t->params->angryProb >= 2.0f)) {
                BtlSpecialIntroStart(arg0);
                MemHeapSelect(sysState.battleHeap);
                return;
            }
        }
        if (MgcForceEnd() != 0) {
            if (t->mgcProg != 0) {
                if (t->id < 4
                    && (ptyPartyData->moveLevels[BtlSpecialMoveRecordIndex(t->skillId)].level == 0 || t->params->angryProb >= 2.0f)) {
                    BtlCaptionShow(ptyMoveTable[t->skillId].name, 0xB4, arg0);
                }
                *(s8 *)&arg0->headLook |= 2;
                if (t->comboPartners[0] >= 0) {
                    for (i = 0; i < 3; i++) {
                        if (t->comboPartners[i] >= 0) {
                            btlUnits[t->comboPartners[i]].headLook |= 2;
                        }
                    }
                }
                MgcStart(t->skillId, t, t->mgcProg);
                arg0->phase = 0x104;
            } else {
                *(s8 *)&arg0->headLook |= 2;
                if (t->id < 4 && t->action == 2) {
                    MgcStart(ptyItemTable[t->skillId].use->moveId, t, 0);
                } else {
                    MgcStart(t->skillId, t, 0);
                }
                arg0->phase = 0x101;
            }
        }
        break;
    case 0x100:
        BtlChrSetMotion(arg0, 6);
        BtlChrMotionSetFrame(arg0, BtlChrMotionFindKey(arg0, 0x407, 0) - 1.0f);
        arg0->subPhase = 0;
        arg0->phase++;
        break;
    case 0x101:
        break;
    case 0x102:
        BtlUnitTurnToTarget(arg0);
        if (arg0->subPhase == 0) {
            if (t->id < 4 && t->action == 2) {
                s = t->def->motionEvents[11];
            } else {
                a = t->def;
                if (a->motionEvents[4] == a->motionEvents[0]) {
                    s = arg0->def->motionEvents[4];
                } else {
                    s = a->motionEvents[4];
                }
            }
            MgcSetMotionTime(BtlMotionKeyGetFrame(s, 3));
            MgcSwitchProc();
            arg0->subPhase++;
        }
        break;
    case 0x103:
        switch (arg0->subPhase) {
        case 0:
            if (t->target != arg0->id) {
                o = &btlUnits[t->target];
                arg0->rot[1] = BtlAtan2Angle(o->pos[0] - arg0->pos[0], o->pos[2] - arg0->pos[2]);
            }
            if (t->skillId >= 1 && t->skillId <= 0x49) {
                if (t->id >= 4) {
                    switch (arg0->params->id) {
                    case 0xA1:
                        SndStreamPlay(((s16 (*)[2])btlVoiceTable)[8 * 0x49 + t->skillId - 1][RNDBIT()], 0, 0);
                        break;
                    case 0x1A1:
                        SndStreamPlay(((s16 (*)[2])btlVoiceTable)[9 * 0x49 + t->skillId - 1][RNDBIT()], 0, 0);
                        break;
                    case 0xC1:
                        SndStreamPlay(((s16 (*)[2])btlVoiceTable)[10 * 0x49 + t->skillId - 1][RNDBIT()], 0, 0);
                        break;
                    case 0xD1:
                        SndStreamPlay(((s16 (*)[2])btlVoiceTable)[11 * 0x49 + t->skillId - 1][RNDBIT()], 0, 0);
                        break;
                    }
                } else if (t->action == 3) {
                    SndStreamPlay(((s16 (*)[2])btlVoiceTable)[(t->params->id - 0x1001) * 0x49 + t->skillId - 1][RNDBIT()], 0, 0);
                }
            }
            BtlChrSetMotionOf(arg0, t, (t->id < 4 && t->action == 2) ? 0xB : 4);
            arg0->evtLoop = NULL;
            arg0->loopStart = arg0->loopEnd;
            arg0->subPhase++;
        case 1:
            if (MgcGetMotionTime() <= 0.0f || BtlChrMotionGetEvent(arg0, 3, 0) != 0) {
                MgcSwitchProc();
                arg0->subPhase = 0;
                arg0->phase++;
            }
            break;
        }
        break;
    case 0x104:
        if (t->mgcProg == 0) {
            BtlUnitTurnToTarget(arg0);
        }
        break;
    case 0x105:
        if (t->mgcProg == 0 && arg0->loopCount >= 0) {
            arg0->loopCount = -1;
            break;
        }
        if ((arg0->frame == arg0->loopStart || t->mgcProg != 0) && BtlUnitsNoScriptedReaction() != 0) {
            FxTraceStopAll(arg0->id);
            if (t->mgcProg == 0 && !(t->id < 4 && t->action == 2)) {
                MgcApplyHitDamage();
                if (--t->combo > 0 && MgcStartNextHit() != 0) {
                    MemHeapSelect(sysState.battleHeap);
                    return;
                }
            }
            info = t->params;
            if (info->angryProb >= 2.0f) {
                info->angryProb = 0.0f;
                *(f32 *)&t->scopeType = 0.0f;
                info->angry = 0;
            } else {
                t->ipMode = 1;
                t->ipPos = t->ipRet;
                t->flags &= 0xFFFFFF7F;
                t->ipSpeed = 0.0f;
                *(f32 *)&t->scopeType = 0.0f;
            }
            BtlChrSetMotion(arg0, 0);
            BtlUnitSetState(t, 0x80);
            if (t->id < 4 || t->action != 5) {
                t->action = 0x7F;
            }
            if (t->id != arg0->id) {
                BtlUnitSetState(arg0, arg0->rtAct);
                arg0->phase = arg0->rtP;
            }
            arg0->partsChr = -1;
            MgcSwitchProc();
            if (t->mgcProg == 0) {
                *(s8 *)&arg0->headLook &= 0xFD;
                VuVecCopy(arg0->moveTarget, arg0->pos);
                if (btlActItemUsedUp != 0) {
                    BtlNoticeBrokenShow(ptyItemTable[arg0->skillId].name);
                }
            } else {
                BtlUnitSetFreeSpotTarget(arg0);
                *(s8 *)&arg0->headLook &= 0xFD;
                if (t->id < 4) {
                    FxWeaponAuraSetKind(t, t->params->eqpAttr);
                }
                if (t->comboPartners[0] >= 0) {
                    for (j = 0; j < 3; j++) {
                        if (t->comboPartners[j] >= 0) {
                            m = &btlUnits[t->comboPartners[j]];
                            m->comboLeader = -1;
                            BtlChrSetMotion(m, 0);
                            m->ipMode = 1;
                            m->ipPos = m->ipRet;
                            m->flags &= 0xFFFFFF7F;
                            m->ipSpeed = 0.0f;
                            *(f32 *)&m->scopeType = 0.0f;
                            BtlUnitSetFreeSpotTarget(m);
                            BtlUnitSetState(m, 0x80);
                            m->action = 0x7F;
                            if (m->id < 4) {
                                btlSpecialUsedMask |= 1 << m->id;
                                FxWeaponAuraSetKind(m, m->params->eqpAttr);
                            }
                            *(s8 *)&m->headLook &= ~2;
                            t->comboPartners[j] = -1;
                        }
                    }
                }
                if (t->id < 4) {
                    btlSpecialUsedMask |= 1 << t->id;
                    if (ptyPartyData->moveLevels[BtlSpecialMoveRecordIndex(t->skillId)].level == 0) {
                        BtlNoticeLearnedShow(ptyMoveTable[t->skillId].name);
                        ptyPartyData->moveLevels[BtlSpecialMoveRecordIndex(t->skillId)].level = 1;
                        BtlComboLevelsUpdate();
                    }
                }
                t->mgcProg = NULL;
            }
            if (btlActingUnit != 0xFD) {
                BtlSchedActDone(arg0->id);
            }
            EnvDimStart(0, 0x20, 1.0f);
            ((void (*)(BtlUnit *, s32, f32, s32))BtlUnitFadeTo)(NULL, 7, 1.0f, 0x20); /* float-first order: the original load schedule */
            btlCamParam.flags |= 2;
            SndBgmFadeVolume(btlBgmHandle.b, 0x96, 100.0f);
            BtlMotionSeSetMuteRange(0, 0);
            EnvFlareFade(0x20, 1.0f);
        }
        break;
    }
    MemHeapSelect(sysState.battleHeap);
}
