/* cc: ee-gcc2.9-991111-01 */
/* as: ee-gcc2.9-991111-01 */
#include "common.h"
#include "protos/battle.h"
#include "party.h"
#include "battle/n33.h"



void BtlUnitStateCast(BtlUnit *arg0) {
    PtyUnit *s1;
    PtyUnit *s2;
    PtyUnit *s3;
    PtyUnit *s4;
    PtyMoveLevelData *e1;
    PtyMoveLevelData *e2;
    PtyMoveLevelData *e3;
    BtlCharDef *x;
    BtlAction *it;
    BtlUnit *u1;
    BtlUnit *u2;
    BtlUnit *u3;
    s8 *p;
    s32 i;
    s32 j1;
    s32 j2;
    s32 j3;
    s32 k;
    s32 kill;
    f32 n;
    f32 f;

    if (arg0->ipMode == 0) {
        if (arg0->body < 0 || btlUnits[arg0->body].partsChr != arg0->id) {
            BtlUnitSetState(arg0, 0x80);
            arg0->action = 0x7F;
        }
        return;
    }
    if (arg0->subPhase == 0) {
        if (arg0->action == 6) {
            if (arg0->id >= 4 ||
                ptyPartyData->moveLevels[BtlSpecialMoveRecordIndex(btlUnits[arg0->comboLeader].skillId)].level != 0) {
                FxCastAuraSet(arg0, 1);
            }
            if (arg0->parts[0] < 0) {
                BtlChrSetMotion(arg0, 5);
            } else if (arg0->motionNo != 0) {
                BtlChrSetMotion(arg0, 0);
            }
        } else if (arg0->id < 4) {
            switch (arg0->action) {
            case 0:
            case 1:
                if (arg0->action == 0) {
                    arg0->combo = arg0->params->comboMax;
                } else {
                    arg0->combo = 1;
                }
            case 5:
                e1 = &ptyMoveTable[arg0->skillId].levels[arg0->skillLevel - 1];
                arg0->ipSpeed = 1.0f / e1->unkE;
                arg0->ipRet = e1->unk4[0] * 0.0001f;
                BtlChrSetMotion(arg0, 5);
                break;
            case 2:
                s1 = arg0->params;
                arg0->ipSpeed = s1->itmSpdRate / ptyItemTable[arg0->skillId].use->unk8;
                arg0->ipRet = s1->restartItem;
                BtlChrSetMotion(arg0, 5);
                break;
            case 3:
                arg0->ipSpeed = 0.0f;
                arg0->ipRet = 0.0f;
                for (i = 0; i < arg0->combo; i++) {
                    if ((u16)arg0->eggNo[i] < 0x18) {
                        arg0->ipSpeed += BtlSpellGetLevelData(arg0, i, 0x20)->unkE;
                        arg0->ipRet += BtlSpellGetLevelData(arg0, i, 0x20)->unk4[0] * 0.0001f;
                    }
                }
                s2 = arg0->params;
                n = arg0->combo;
                f = arg0->ipRet / n;
                arg0->ipSpeed = s2->mgcSpdRate * n / arg0->ipSpeed;
                arg0->ipRet = f;
                arg0->ipRet = f * s2->mgcRetRate;
                BtlChrSetMotion(arg0, 6);
                FxCastAuraSet(arg0, 2);
                break;
            case 4:
                s3 = arg0->params;
                e2 = &ptyMoveTable[arg0->skillId].levels[arg0->skillLevel - 1];
                arg0->ipSpeed = s3->spcSpdRate / e2->unkE;
                arg0->ipRet = e2->unk4[0] * 0.0001f * s3->spcRetRate;
                if (arg0->comboPartners[0] >= 0) {
                    for (j1 = 0; j1 < 3; j1++) {
                        if (arg0->comboPartners[j1] >= 0) {
                            u1 = &btlUnits[arg0->comboPartners[j1]];
                            u1->ipRet = e2->unk4[j1 + 1] * 0.0001f * u1->params->spcRetRate;
                        }
                    }
                }
                BtlChrSetMotion(arg0, 5);
                FxCastAuraSet(arg0, 1);
                break;
            }
        } else {
            x = arg0->def;
            it = x->data->actionSets[arg0->ai].actions[arg0->action];
            e3 = it->level;
            arg0->ipSpeed = 1.0f / e3->unkE;
            arg0->ipRet = e3->unk4[0] * 0.0001f;
            if (arg0->comboPartners[0] >= 0) {
                for (j2 = 0; j2 < 3; j2++) {
                    if (arg0->comboPartners[j2] >= 0) {
                        u2 = &btlUnits[arg0->comboPartners[j2]];
                        u2->ipRet = e3->unk4[j2 + 1] * 0.0001f * u2->params->spcRetRate;
                    }
                }
            }
            if (it->unk4 == 0) {
                FxCastAuraSet(arg0, e3->mpCost > 0 ? 2 : 1);
                arg0->combo = 1;
            } else {
                arg0->combo = arg0->params->comboMax + x->unk6;
            }
            if (it->unk4 == 0 && arg0->parts[0] < 0) {
                BtlChrSetMotion(arg0, 5);
            } else if (arg0->motionNo != 0) {
                BtlChrSetMotion(arg0, 0);
            }
        }
        arg0->subPhase++;
    }
    if (arg0->comboPartners[0] >= 0 && (arg0->flags & 0x80)) {
        for (k = 0; k < 3; k++) {
            if (arg0->comboPartners[k] >= 0) {
                u3 = &btlUnits[arg0->comboPartners[k]];
                if (u3->ipMode != 2 || u3->action != 6) {
                    break;
                }
            }
        }
        if (k >= 3) {
            p = arg0->comboPartners;
            kill = arg0->id < 4 && ptyPartyData->moveLevels[BtlSpecialMoveRecordIndex(arg0->skillId)].level == 0;
            arg0->flags &= 0xFFFFFF7F;
            if (kill) {
                arg0->ipPos = 0.0f;
            }
            for (j3 = 0; j3 < 3; j3++) {
                if (p[j3] >= 0) {
                    btlUnits[p[j3]].ipSpeed = arg0->ipSpeed;
                    btlUnits[p[j3]].flags &= 0xFFFFFF7F;
                    if (kill) {
                        btlUnits[p[j3]].ipPos = 0.0f;
                    }
                }
            }
        }
    }
    if (arg0->parts[0] >= 0 || arg0->body >= 0 || arg0->action == 6) {
        if (btlActingUnit == 0xFF) {
            if ((arg0->params->bitCsf & 0x10) && arg0->ipPos > 0.0f && arg0->body < 0) {
                if (arg0->motionNo != 3) {
                    BtlChrSetMotion(arg0, 3);
                }
                if (BtlUnitMoveToward(arg0, &arg0->moveTarget, 3, BtlUnitGetMoveSpeed(arg0), 10.0f)) {
                    BtlUnitSetFreeSpotTarget(arg0);
                }
            } else if (arg0->action == 6) {
                BtlUnitTurnToTarget(arg0);
            }
        }
    } else {
        s4 = arg0->params;
        if (((s4->status & 0x100) || (s4->bitCsf & 0x10)) && arg0->ipPos > 0.0f) {
            if (arg0->motionNo != 3) {
                BtlChrSetMotion(arg0, 3);
            }
            if (BtlUnitMoveToward(arg0, &arg0->moveTarget, 3, BtlUnitGetMoveSpeed(arg0), 10.0f)) {
                BtlUnitSetFreeSpotTarget(arg0);
            }
        } else {
            BtlUnitTurnToTarget(arg0);
        }
        if (arg0->ipPos <= 0.0f && !(arg0->flags & 0x80)) {
            if (btlActingUnit == arg0->id) {
                if (BtlUnitCommandValidate(arg0)) {
                    BtlUnitActionBegin(arg0);
                    BtlUnitSetState(arg0, 0xC1);
                    return;
                }
                BtlSchedActDone(arg0->id);
                return;
            } else {
                switch (BtlUnitCommandValidate(arg0)) {
                case 1:
                    BtlUnitSetState(arg0, 0xC1);
                    break;
                case 2:
                    BtlSchedActRequest(arg0->id);
                    break;
                }
            }
        }
    }
}
