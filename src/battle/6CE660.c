/* as: ee-gcc2.9-991111-01 */
#include "common.h"
#include "protos/battle.h"
#include "battle/misc.h"
#include "battle/n33.h"
#include "party.h"

#define CLAMP(x, max) (x) = (x) < 0 ? 0 : ((x) > (max) ? (max) : (x))
#define CLAMP32(x, max) (x) = (x) > 0 ? ((x) > (max) ? (max) : (x)) : 1
#define CEIL(x) ((x) == (s32)(x) ? (x) : ((x) > 0 ? (s32)(x) + 1 : ~(s32)(-x)))



void BtlUnitActionBegin(BtlUnit *arg0) {
    PtyItem *e;
    PtyUnit *st;
    PtyMove *t;
    BtlUnit *u;
    BtlUnit *u2;
    BtlAction *sl;
    PtyMoveLevelData *c;
    PtyManaEgg *g;
    PtyMoveLevel *w;
    s32 i;
    s32 j;
    s32 j2;
    s32 cost;
    s16 k;
    PtyMoveLevelThreshold *an;
    s8 chance;
    PtyChar *r;
    PtyItemUse *sp;

    BtlUnitCollectAreaTargetsAtTarget(arg0);
    FxCastAuraSet(arg0, -1);
    btlActItemUsedUp = 0;
    if (arg0->id < 4) {
        switch (arg0->action) {
        case 2:
            e = &ptyItemTable[arg0->skillId];
            if (e->equip != 0) {
                sp = e->use;
                if (!(arg0->params->bitCsf & 0x02000000)) {
                    chance = sp->rate;
                    if (chance >= 100 ||
                        chance * 0.01f > (rand() >> 15) * (1.0f / 65536.0f)) {
                        r = &ptyCharData[arg0->params->id - 0x1001];
                        r->equip[e->type - 5] = 0;
                        BtlUnitRecalcParams(arg0);
                        btlActItemUsedUp = 1;
                    }
                }
            } else {
                btlActItemUsedUp = BtlCmdListItemConsume(arg0->id);
            }
            BtlCaptionShow(e->name, 0xB4, arg0);
            arg0->mvp += 5;
            EnvDimForSkill(arg0, ptyMoveTable[((PtyItemUse *)*(s32 *)&ptyItemTable[arg0->skillId].use)->moveId].unkF); /* s32 load of use: alias order */
            break;
        case 3:
            for (i = 0; i < arg0->combo; i++) {
                if ((u16)arg0->eggNo[i] < 0x18) {
                    g = &ptyPartyData->eggs[(s16)arg0->eggNo[i]];
                    g->mp -= ((s32 (*)(BtlUnit *, s16, s8))BtlUnitManaEggSpellCost)(arg0, (u16)arg0->eggNo[i], arg0->eggMagic[i]); /* cast: s16/s8 parameters */
                    CLAMP(g->mp, g->maxMp);
                }
            }
            arg0->mvp += 5;
            BtlCaptionShow(ptyMoveTable[arg0->skillId].name, 0xB4, arg0);
            EnvDimForSkill(arg0, ptyMoveTable[arg0->skillId].unkF);
            break;
        case 4:
            t = &ptyMoveTable[arg0->skillId];
            if (t->icon >= 1 && t->icon <= 8) {
                FxWeaponAuraSetKind(arg0, t->icon);
            } else {
                FxWeaponAuraSetKind(arg0, 0);
            }
            if (arg0->params->angryProb < 2.0f) {
                cost = CEIL(t->levels[arg0->skillLevel - 1].cost * arg0->params->spcSpRate);
                if (ptyPartyData->moveLevels[BtlSpecialMoveRecordIndex(arg0->skillId)].level != 0) {
                    st = arg0->params;
                    st->sp -= cost;
                    CLAMP(st->sp, st->maxSp);
                }
                if (arg0->comboPartners[0] >= 0) {
                    for (j = 0; j < 3; j++) {
                        if (arg0->comboPartners[j] >= 0) {
                            u = &btlUnits[arg0->comboPartners[j]];
                            if (ptyPartyData->moveLevels[BtlSpecialMoveRecordIndex(arg0->skillId)].level != 0) {
                                st = u->params;
                                st->sp -= cost;
                                CLAMP(st->sp, st->maxSp);
                            }
                            FxCastAuraSet(u, -1);
                            u->mvp += 15;
                            if (t->icon >= 1 && t->icon <= 8) {
                                FxWeaponAuraSetKind(u, t->icon);
                            } else {
                                FxWeaponAuraSetKind(u, 0);
                            }
                        }
                    }
                    arg0->mvp += 15;
                } else {
                    arg0->mvp += 5;
                }
            }
            if (ptyPartyData->moveLevels[BtlSpecialMoveRecordIndex(arg0->skillId)].level != 0 && arg0->params->angryProb < 2.0f) {
                BtlCaptionShow(t->name, 0xB4, arg0);
            }
            if ((u32)(arg0->skillId - 0x65) < 0x2F) {
                k = BtlSpecialMoveRecordIndex(arg0->skillId);
                w = &ptyPartyData->moveLevels[k];
                w->exp++;
                CLAMP32(w->exp, 9999);
                if (w->level != 0 && w->level < w->maxLevel) {
                    an = &ptyMoveLevelThresholds[k];
                    if (w->exp >= an->exp[w->level]) {
                        arg0->skillLevel = ++w->level;
                        BtlComboLevelsUpdate();
                        BtlCaptionLevelUpStamp();
                    }
                }
            }
            EnvDimForSkill(arg0, ptyMoveTable[arg0->skillId].unkF);
            break;
        case 5:
            BtlCamSetMode(0, 0x10, arg0, BtlCamEscapeCb);
            btlPhase = 0x82;
            break;
        }
    } else {
        sl = BTL_UNIT_ACT(arg0);
        c = sl->level;
        arg0->params->mp -= CEIL(c->mpCost * arg0->params->mgcMpRate);
        CLAMP(arg0->params->mp, arg0->params->maxMp);
        arg0->params->sp -= CEIL(c->cost * arg0->params->spcSpRate);
        CLAMP(arg0->params->sp, arg0->params->maxSp);
        if (arg0->comboPartners[0] >= 0) {
            for (j2 = 0; j2 < 3; j2++) {
                if (arg0->comboPartners[j2] >= 0) {
                    u2 = &btlUnits[arg0->comboPartners[j2]];
                    u2->params->mp -= CEIL(c->mpCost * u2->params->mgcMpRate);
                    CLAMP(u2->params->mp, u2->params->maxMp);
                    u2->params->sp -= CEIL(c->cost * u2->params->spcSpRate);
                    CLAMP(u2->params->sp, u2->params->maxSp);
                    FxCastAuraSet(u2, -1);
                    u2->skillId = arg0->skillId;
                }
            }
        }
        BtlCaptionShow(sl->name, 0xB4, arg0);
        if (sl->moveId < 0x52) {
            EnvDimForSkill(arg0, ptyMoveTable[sl->moveId].unkF);
        } else {
            EnvDimForSkill(arg0, sl->dimKind);
        }
        if (arg0->def->data->actionSets[arg0->ai].type == 0x47) {
            arg0->flag |= 0x08000000;
        }
    }
    SndBgmFadeVolume(btlBgmHandle.b, 0x96, 80.0f);
}
