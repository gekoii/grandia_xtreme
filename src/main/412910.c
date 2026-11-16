/* cc: ee-gcc2.9-991111-01 */
/* as: ee-gcc2.9-991111-01 */
#include "common.h"
#include "protos/main.h"
#include "party.h"
#include "main/n01.h"


#define ROUNDUP(x) ((x) == (s32)(x) ? (x) : ((x) > 0 ? (s32)(x) + 1 : ~(s32)(-x)))
#define CLAMP(x, max) (x) = (x) < 0 ? 0 : ((x) > (max) ? (max) : (x))
#define ADJ(f, v, cap, bias) \
    (f) = (PtySoftCap(v, cap) + (bias)) * 0.01f; \
    if ((f) < 0.0f) (f) = 0.0f
#define ADJN(f, v, bias) \
    (f) = ((v) + (bias)) * 0.01f; \
    if ((f) < 0.0f) (f) = 0.0f
#define RECIP(f) if ((f) <= 0.0f) (f) = 99999.9f; else (f) = 1.0f / (f)

void PtyUnitCalcParams(PtyUnit *p, PtyEnemyData *e, PtyEnemyParams *x) {
    s32 r0, r1, r2, r3, i, j, k, n, m, q, ei, ej, en, em, eq, t0;

    p->bitCsf &= 0xFFFF;
    p->exeSp = p->exeGood = p->exeBad = p->kregGood = p->kregSpi = p->kregSeal = p->kregOther = p->eqpKiller = p->eqpBitSta = p->eqpAttr = p->crtclMulti =
        p->params[0] = p->params[1] = p->params[2] = p->params[3] = p->params[4] = p->params[5] = p->status2 = p->maxHp = p->maxMp = p->maxSp = p->stats[0] =
            p->stats[1] = p->stats[2] = p->stats[3] = p->stats[4] = p->stats[5] = p->eqpStaProb = 0;
    for (r0 = 0; r0 < 5; r0++) {
        p->kregPowD[r0] = p->kregPowU[r0] = 0;
    }
    for (r1 = 0; r1 < 13; r1++) {
        p->killer[r1] = p->regRace[r1] = p->defRace[r1] = 0.0f;
    }
    p->comboMax = p->spSupCmb = p->spSupCrt = p->absorbHp = p->crtclUp = p->spcSpdRate = p->spcRetRate = p->spcSpRate = p->mgcSpdRate = p->mgcRetRate = p->mgcMpRate =
        p->spcPow = 0;
    for (r2 = 0; r2 < 9; r2++) {
        p->magicRate[r2] = p->magicPow[r2] = p->regMgcAttr[r2] = 0;
    }
    p->mgc1IpCan = p->multiMagic = p->itmSpdRate = p->restartItem = p->itemUpBomb = p->itemUpScr = p->itemUpDrug = p->guardDec = p->guardHp = p->guardSp = p->escapeUp = 0;
    for (r3 = 11; r3 >= 0; r3--) {
        p->regSta[r3] = 0;
    }
    p->cmdDisable = p->atkDown = p->defDown = p->startIpOfs = p->mcAwayDec = p->comboUp = p->avoidWait = p->avoidEvade = p->strikeBack = p->noDamage = p->refDamage =
        p->regIp = p->spSupDmg = p->spSupDown = p->spSupGuard = p->takeDmgMp = p->autoGuard = p->autoYomi = p->angryRate = p->sArmorRate = p->lure =
            p->antiLvDrain = p->antiKb = p->antiRetire = p->antiCounter = p->antiCancel = p->antiHalf = p->getGoldRate = p->getRateUp = p->getItemUp = p->getExpRate =
                p->combiHint = p->mcActDisp = p->lvupMhp = p->lvupStats[0] = p->lvupStats[1] = p->lvupStats[2] = p->lvupStats[3] = p->lvupStats[4] = p->atkRef =
                    p->startTransp = p->startSleep = p->startPara = p->lvupStats[5] = 0;

    if (e == NULL) {
        PtyChar *s;
        PtyCharDef *r;

        ptyApplyingEquip = 1;
        s = &ptyCharData[p->id - 0x1001];
        for (i = 0; i < 6; i++) {
            if (s->equip[i] > 0) {
                PtyEquipData *it = ptyItemTable[s->equip[i]].equip;
                p->params[0] += it->stats[0];
                p->params[1] += it->stats[1];
                p->params[2] += it->stats[2];
                p->params[3] += it->stats[3];
                p->params[4] += it->stats[4];
                p->params[5] += it->stats[5];
                if (!(p->status & 0x4000)) {
                    if (i == 0) {
                        p->eqpAttr = it->unkF;
                        p->eqpKiller = it->unkE;
                        p->eqpBitSta = it->unk10;
                        p->eqpStaProb = it->unk14 * 0.01f;
                    } else {
                        for (j = 0; j < 9; j++) {
                            p->regMgcAttr[j] += it->elements[j];
                        }
                        for (j = 0; j < 12; j++) {
                            p->regSta[j] += (it->unk10 & (0x10 << j)) ? it->unk14 * 1.5f : 0.0f;
                        }
                    }
                    if (it->effectType != 0) {
                        PtyUnitApplyEffect(p, it->effectType, it->effectValue);
                    }
                }
            }
        }
        if (!(p->status & 0x4000)) {
            ptyApplyingEquip = 0;
            for (i = 0; i < 15; i++) {
                if (s->skills[i] >= 0) {
                    PtySkill *sl = &ptyPartyData->skills[s->skills[i]];
                    if (sl->level > 0) {
                        PtySkillDef *b = &ptySkillTable[sl->id];
                        for (k = 0; k < 3; k++) {
                            PtyUnitApplyEffect(p, b->effects[k], b->amount * sl->level);
                        }
                    }
                }
            }
        }
        p->maxHp = ROUNDUP(s->maxHp * (p->maxHp * 0.01f));
        p->maxHp += s->maxHp;
        CLAMP(p->maxHp, 99999);
        p->maxSp = ROUNDUP(s->maxSp * (p->maxSp * 0.01f));
        p->maxSp += s->maxSp;
        CLAMP(p->maxSp, 999);
        p->stats[0] = ROUNDUP(s->stats[0] * (p->stats[0] * 0.01f));
        p->stats[0] += s->stats[0];
        CLAMP(p->stats[0], 9999);
        p->stats[1] = ROUNDUP(s->stats[1] * (p->stats[1] * 0.01f));
        p->stats[1] += s->stats[1];
        CLAMP(p->stats[1], 9999);
        p->stats[2] = ROUNDUP(s->stats[2] * (p->stats[2] * 0.01f));
        p->stats[2] += s->stats[2];
        CLAMP(p->stats[2], 9999);
        p->stats[3] = ROUNDUP(s->stats[3] * (p->stats[3] * 0.01f));
        p->stats[3] += s->stats[3];
        CLAMP(p->stats[3], 9999);
        p->stats[4] = ROUNDUP(s->stats[4] * (p->stats[4] * 0.01f));
        p->stats[4] += s->stats[4];
        CLAMP(p->stats[4], 9999);
        p->stats[5] = ROUNDUP(s->stats[5] * (p->stats[5] * 0.01f));
        p->stats[5] += s->stats[5];
        CLAMP(p->stats[5], 9999);

        r = &ptyCharDefTable[p->id - 0x1001];
        p->exeSp = ROUNDUP(r->unk2C[0] * (PtySoftCap(p->exeSp, 75.0f) * 0.01f));
        p->exeSp += r->unk2C[0];
        CLAMP(p->exeSp, 9999);
        p->exeGood = ROUNDUP(r->unk2C[1] * (PtySoftCap(p->exeGood, 75.0f) * 0.01f));
        p->exeGood += r->unk2C[1];
        CLAMP(p->exeGood, 9999);
        p->exeBad = ROUNDUP(r->unk2C[2] * (PtySoftCap(p->exeBad, 75.0f) * 0.01f));
        p->exeBad += r->unk2C[2];
        CLAMP(p->exeBad, 9999);

        ADJ(p->kregGood, -(p->kregGood + r->unk3E[0]), 100.0f, 100.0f);
        ADJ(p->kregSpi, -(p->kregSpi + r->unk3E[1]), 100.0f, 100.0f);
        ADJ(p->kregSeal, -(p->kregSeal + r->unk3E[2]), 100.0f, 100.0f);
        ADJ(p->kregOther, -(p->kregOther + r->unk3E[3]), 100.0f, 100.0f);
        for (n = 0; n < 5; n++) {
            ADJ(p->kregPowD[n], -(p->kregPowD[n] + r->unk42[n]), 100.0f, 100.0f);
            ADJ(p->kregPowU[n], -(p->kregPowU[n] - r->unk42[n]), 100.0f, 100.0f);
        }
        p->comboMax += r->unk47;
        p->spSupCmb += r->unk32[0];
        p->spSupCrt += r->unk32[1];
        p->restartItem = (100.0f - PtySoftCap(p->restartItem, 50.0f)) * 0.01f * r->unk20 * 0.0001f;
        ADJ(p->guardDec, -(p->guardDec + r->unk1), 90.0f, 100.0f);
        ADJ(p->avoidWait, p->avoidWait + r->unk2, 100.0f, 0.0f);
        ADJ(p->avoidEvade, p->avoidEvade + r->unk3, 100.0f, 0.0f);
        ADJ(p->regIp, -(p->regIp + r->unk3C), 75.0f, 100.0f);
        p->spSupDmg += r->unk32[2];
        p->spSupDown += r->unk32[3];
        p->spSupGuard += r->unk32[4];
        for (m = 0; m < 9; m++) {
            ADJN(p->magicRate[m], p->magicRate[m], 100.0f);
            p->regMgcAttr[m] = (100.0f - p->regMgcAttr[m]) * 0.01f;
            if (p->regMgcAttr[m] > 2.0f) {
                p->regMgcAttr[m] = 2.0f;
            } else if (p->regMgcAttr[m] <= 0.0f) {
                p->regMgcAttr[m] = 0.0f;
            }
        }
        for (q = 0; q < 12; q++) {
            ADJ(p->regSta[q], -p->regSta[q], 100.0f, 100.0f);
        }
    } else {
        if (!(p->status & 0x4000)) {
            ptyApplyingEquip = 1;
            for (ei = 0; ei < 2; ei++) {
                if (x->effectTypes[ei] != 0) {
                    PtyUnitApplyEffect(p, x->effectTypes[ei], x->effectValues[ei]);
                }
            }
            ptyApplyingEquip = 0;
            for (ei = 0; ei < 6; ei++) {
                if (x->skills[ei] > 0 && x->skillLevels[ei] > 0) {
                    PtySkillDef *b = &ptySkillTable[x->skills[ei]];
                    for (ej = 0; ej < 3; ej++) {
                        PtyUnitApplyEffect(p, b->effects[ej], b->amount * x->skillLevels[ei]);
                    }
                }
            }
        }
        p->stats[0] = p->stats[0] * (e->statRates[0] * 0.01f);
        p->stats[1] = p->stats[1] * (e->statRates[1] * 0.01f);
        p->stats[2] = p->stats[2] * (e->statRates[2] * 0.01f);
        p->stats[3] = p->stats[3] * (e->statRates[3] * 0.01f);
        p->stats[4] = p->stats[4] * (e->statRates[4] * 0.01f);
        p->stats[5] = p->stats[5] * (e->statRates[5] * 0.01f);
        p->maxHp = ROUNDUP(x->maxHp * (p->maxHp * 0.01f));
        p->maxHp += x->maxHp;
        CLAMP(p->maxHp, 9999999);
        p->maxMp = ROUNDUP(x->maxMp * (p->maxMp * 0.01f));
        p->maxMp += x->maxMp;
        CLAMP(p->maxMp, 99999);
        p->maxSp = ROUNDUP(x->maxSp * (p->maxSp * 0.01f));
        p->maxSp += x->maxSp;
        CLAMP(p->maxSp, 99999);
        p->stats[0] = ROUNDUP(x->stats[0] * (p->stats[0] * 0.01f));
        p->stats[0] += x->stats[0];
        CLAMP(p->stats[0], 99999);
        p->stats[1] = ROUNDUP(x->stats[1] * (p->stats[1] * 0.01f));
        p->stats[1] += x->stats[1];
        CLAMP(p->stats[1], 99999);
        p->stats[2] = ROUNDUP(x->stats[2] * (p->stats[2] * 0.01f));
        p->stats[2] += x->stats[2];
        CLAMP(p->stats[2], 99999);
        p->stats[3] = ROUNDUP(x->stats[3] * (p->stats[3] * 0.01f));
        p->stats[3] += x->stats[3];
        CLAMP(p->stats[3], 99999);
        p->stats[4] = ROUNDUP(x->stats[4] * (p->stats[4] * 0.01f));
        p->stats[4] += x->stats[4];
        CLAMP(p->stats[4], 99999);
        p->stats[5] = ROUNDUP(x->stats[5] * (p->stats[5] * 0.01f));
        p->stats[5] += x->stats[5];
        CLAMP(p->stats[5], 99999);

        p->antiHalf += e->antiHalf;
        p->exeSp = ROUNDUP(e->exeSp * (PtySoftCap(p->exeSp, 75.0f) * 0.01f));
        p->exeSp += e->exeSp;
        CLAMP(p->exeSp, 9999);
        p->exeGood = ROUNDUP(e->exeGood * (PtySoftCap(p->exeGood, 75.0f) * 0.01f));
        p->exeGood += e->exeGood;
        CLAMP(p->exeGood, 9999);
        p->exeBad = ROUNDUP(e->exeBad * (PtySoftCap(p->exeBad, 75.0f) * 0.01f));
        p->exeBad += e->exeBad;
        CLAMP(p->exeBad, 9999);

        ADJ(p->kregGood, (p->status & 0x4000) ? 0.0f : -(p->kregGood + e->kregGood), 100.0f, 100.0f);
        ADJ(p->kregSpi, (p->status & 0x4000) ? 0.0f : -(p->kregSpi + e->kregSpi), 100.0f, 100.0f);
        ADJ(p->kregSeal, (p->status & 0x4000) ? 0.0f : -(p->kregSeal + e->kregSeal), 100.0f, 100.0f);
        ADJ(p->kregOther, (p->status & 0x4000) ? 0.0f : -(p->kregOther + e->kregOther), 100.0f, 100.0f);
        for (en = 0; en < 5; en++) {
            ADJ(p->kregPowD[en], (p->status & 0x4000) ? ptyNegZeroF : -(p->kregPowD[en] + e->kregPow[en]), 75.0f, 100.0f);
            ADJ(p->kregPowU[en], (p->status & 0x4000) ? ptyNegZeroF : -(p->kregPowU[en] - e->kregPow[en]), 100.0f, 100.0f);
        }
        p->comboMax += 1;
        ADJ(p->guardDec, -p->guardDec, 90.0f, 100.0f);
        ADJ(p->avoidWait, (p->status & 0x4000) ? 0.0f : p->avoidWait + e->avoid, 100.0f, 0.0f);
        ADJ(p->avoidEvade, (p->status & 0x4000) ? 0.0f : p->avoidEvade + e->avoid, 100.0f, 0.0f);
        ADJ(p->regIp, (p->status & 0x4000) ? 0.0f : -(p->regIp + e->regIp), 75.0f, 100.0f);
        for (em = 0; em < 9; em++) {
            p->magicRate[em] = !(p->status & 0x4000) ? (p->magicRate[em] + e->magicRate[em] + 100.0f) * 0.01f : 1.0f;
            if (p->magicRate[em] < 0.0f) {
                p->magicRate[em] = 0.0f;
            }
            p->regMgcAttr[em] = !(p->status & 0x4000) ? (100.0f - (p->regMgcAttr[em] + e->regMgcAttr[em])) * 0.01f : 1.0f;
            if (p->regMgcAttr[em] > 2.0f) {
                p->regMgcAttr[em] = 2.0f;
            } else if (p->regMgcAttr[em] <= 0.0f) {
                p->regMgcAttr[em] = 0.0f;
            }
        }
        for (eq = 0; eq < 12; eq++) {
            f32 t = PtySoftCap(-p->regSta[eq], 100.0f);
            p->regSta[eq] = (!(e->staResist & (0x10 << eq)) ? t + 100.0f : t + 0.0f) * 0.01f;
            if (p->regSta[eq] < 0.0f) {
                p->regSta[eq] = 0.0f;
            }
        }
    }

    for (t0 = 0; t0 < 13; t0++) {
        ADJN(p->killer[t0], p->killer[t0], 0.0f);
        ADJN(p->regRace[t0], -p->regRace[t0], 0.0f);
    }
    ADJN(p->absorbHp, p->absorbHp, 0.0f);
    if (p->absorbHp > 1.0f) {
        p->absorbHp = 1.0f;
    }
    ADJ(p->spcSpdRate, -p->spcSpdRate, 75.0f, 100.0f);
    RECIP(p->spcSpdRate);
    ADJ(p->spcRetRate, -p->spcRetRate, 50.0f, 100.0f);
    ADJN(p->spcSpRate, -p->spcSpRate, 100.0f);
    ADJ(p->mgcSpdRate, -p->mgcSpdRate, 75.0f, 100.0f);
    RECIP(p->mgcSpdRate);
    ADJ(p->mgcRetRate, -p->mgcRetRate, 50.0f, 100.0f);
    ADJN(p->mgcMpRate, -p->mgcMpRate, 100.0f);
    p->multiMagic += 1;
    ADJ(p->itmSpdRate, -p->itmSpdRate, 75.0f, 100.0f);
    RECIP(p->itmSpdRate);
    ADJN(p->itemUpBomb, p->itemUpBomb, 100.0f);
    ADJN(p->itemUpScr, p->itemUpScr, 100.0f);
    ADJN(p->itemUpDrug, p->itemUpDrug, 100.0f);
    ADJ(p->escapeUp, p->escapeUp, 100.0f, 0.0f);
    ADJ(p->strikeBack, p->strikeBack, 100.0f, 0.0f);
    ADJ(p->refDamage, p->refDamage, 75.0f, 0.0f);
    ADJN(p->takeDmgMp, p->takeDmgMp, 0.0f);
    ADJ(p->autoGuard, p->autoGuard, 50.0f, 0.0f);
    ADJ(p->autoYomi, p->autoYomi, 50.0f, 0.0f);
    ADJ(p->angryRate, p->angryRate, 100.0f, 0.0f);
    ADJ(p->sArmorRate, p->sArmorRate, 100.0f, 0.0f);
    ADJ(p->lure, p->lure, 50.0f, 0.0f);
    ADJ(p->antiLvDrain, p->antiLvDrain, 100.0f, 0.0f);
    ADJ(p->antiKb, p->antiKb, 100.0f, 0.0f);
    ADJ(p->antiRetire, (p->bitCsf & 2) ? 200.0f : p->antiRetire, 100.0f, 0.0f);
    ADJ(p->antiCounter, p->antiCounter, 100.0f, 0.0f);
    ADJ(p->antiCancel, (p->bitCsf & 1) ? 200.0f : p->antiCancel, 100.0f, 0.0f);
    ADJN(p->antiHalf, p->antiHalf, 0.0f);
    if (p->antiHalf > 1.0f) {
        p->antiHalf = 1.0f;
    }
    ADJN(p->getGoldRate, p->getGoldRate, 0.0f);
    ADJN(p->getRateUp, p->getRateUp, 0.0f);
    ADJN(p->getItemUp, p->getItemUp, 0.0f);
    ADJ(p->getExpRate, p->getExpRate, 100.0f, 100.0f);
    ADJ(p->combiHint, p->combiHint, 100.0f, 0.0f);
    ADJ(p->mcActDisp, p->mcActDisp, 100.0f, 0.0f);
    ADJN(p->atkRef, p->atkRef, 0.0f);
    ADJN(p->startTransp, p->startTransp, 0.0f);
    ADJN(p->startSleep, p->startSleep, 0.0f);
    ADJN(p->startPara, p->startPara, 0.0f);
    ADJ(p->mcAwayDec, p->mcAwayDec, 100.0f, 0.0f);

    p->params[0] += p->stats[0];
    CLAMP(p->params[0], 99999);
    p->params[1] += p->stats[1];
    CLAMP(p->params[1], 99999);
    p->params[2] += p->stats[2];
    CLAMP(p->params[2], 99999);
    p->params[3] += p->stats[3];
    CLAMP(p->params[3], 99999);
    p->params[4] += p->stats[4];
    CLAMP(p->params[4], 99999);
    p->params[5] += p->stats[5];
    CLAMP(p->params[5], 99999);
    CLAMP(p->hp, p->maxHp);
    CLAMP(p->mp, p->maxMp);
    CLAMP(p->sp, p->maxSp);
}
