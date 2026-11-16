#include "common.h"
#include "party.h"

void PtyUnitApplyEffect(PtyUnit *p, s32 type, s32 val) {
    if (type <= 0) {
        return;
    }
    if (type < 11) {
    } else if (type < 28) {
        switch (type) {
        case 11:
            p->maxHp += val;
            return;
        case 12:
            p->maxSp += val;
            return;
        case 13:
            p->stats[0] += val;
            return;
        case 14:
            p->stats[1] += val;
            return;
        case 15:
            p->stats[2] += val;
            return;
        case 16:
            p->stats[3] += val;
            return;
        case 17:
            p->stats[4] += val;
            return;
        case 18:
            p->stats[5] += val;
            return;
        case 19:
            p->exeSp += val;
            return;
        case 20:
            p->exeBad += val;
            return;
        case 21:
            p->kregGood += val;
            return;
        case 22:
            p->kregSpi += val;
            return;
        case 23:
            p->kregSeal += val;
            return;
        case 24:
            {
                s32 i;
                for (i = 0; i < 5; i++) {
                    p->kregPowU[i] += val;
                }
            }
            return;
        case 25:
            {
                s32 i;
                for (i = 0; i < 5; i++) {
                    p->kregPowD[i] += val;
                }
            }
            return;
        case 26:
            p->bitCsf |= 0x80000;
            return;
        }
    } else if (type < 40) {
        p->killer[type - 27] += val;
    } else if (type < 52) {
        if (ptyApplyingEquip == 0) {
            p->regRace[type - 39] += val;
        } else {
            p->defRace[type - 39] += val;
        }
    } else if (type < 66) {
        switch (type) {
        case 0x34:
            p->comboMax += val;
            return;
        case 0x35:
            p->spSupCmb += val;
            p->spSupCrt += val;
            return;
        case 0x36:
            p->absorbHp += val;
            return;
        case 0x37:
            p->crtclUp += val;
            return;
        case 0x38:
            p->crtclMulti += val;
            return;
        case 0x39:
            p->bitCsf |= 0x10000;
            return;
        case 0x3A:
            p->bitCsf |= 0x20000;
            return;
        case 0x3B:
            p->spcSpdRate += val;
            return;
        case 0x3C:
            p->spcRetRate += val;
            return;
        case 0x3D:
            p->spcPow += val;
            return;
        case 0x3E:
            p->spcSpRate += val;
            return;
        case 0x3F:
            p->mgcSpdRate += val;
            return;
        case 0x40:
            p->mgcRetRate += val;
            return;
        case 0x41:
            p->mgcMpRate += val;
            return;
        }
    } else if (type < 0x4B) {
    } else if (type < 0x58) {
        switch (type) {
        case 0x4C:
            p->mgc1IpCan += val;
            return;
        case 0x4D:
            p->multiMagic += val;
            return;
        case 0x4E:
            p->itmSpdRate += val;
            return;
        case 0x51:
            p->itemUpBomb += val;
            return;
        case 0x52:
            p->itemUpScr += val;
            return;
        case 0x53:
            p->itemUpDrug += val;
            return;
        case 0x54:
            p->guardDec += val;
            return;
        case 0x55:
            p->guardHp += val;
            return;
        case 0x56:
            p->guardSp += val;
            return;
        case 0x57:
            p->escapeUp += val;
            return;
        }
    } else if (type < 0x62) {
        if (type == 0x61) {
            p->regSta[11] += val;
        } else {
            p->regSta[type - 0x58] += val;
        }
    } else if (type < 0x64) {
        p->avoidWait += val;
        p->avoidEvade += val;
        if (type == 0x63) {
            p->bitCsf |= 0x40000;
        }
    } else if (type < 0x74) {
        switch (type) {
        case 0x64:
            p->strikeBack += val;
            return;
        case 0x65:
            p->noDamage += val;
            return;
        case 0x66:
            p->refDamage += val;
            return;
        case 0x67:
            p->regIp += val;
            return;
        case 0x68:
            p->spSupDmg += val;
            p->spSupDown += val;
            p->spSupGuard += val;
            return;
        case 0x69:
            p->takeDmgMp += val;
            return;
        case 0x6A:
            p->autoGuard += val;
            return;
        case 0x6B:
            if (ptyApplyingEquip == 0) {
                p->autoYomi += val;
            }
            return;
        case 0x6C:
            p->angryRate += val;
            return;
        case 0x6D:
            p->sArmorRate += val;
            return;
        case 0x6E:
            p->lure += val;
            return;
        case 0x6F:
            p->antiLvDrain += val;
            return;
        case 0x70:
            p->antiKb += val;
            return;
        case 0x71:
            p->antiRetire += val;
            return;
        case 0x72:
            p->antiCounter += val;
            return;
        case 0x73:
            p->antiCancel += val;
            return;
        }
    } else if (type < 0x88) {
        switch (type) {
        case 0x75:
            p->getRateUp += val;
            return;
        case 0x76:
            p->getItemUp += val;
            return;
        case 0x77:
            p->getExpRate += val;
            return;
        case 0x7B:
            p->combiHint += val;
            return;
        case 0x7C:
            p->mcActDisp += val;
            return;
        case 0x81:
            p->lvupMhp += val;
            return;
        case 0x82:
            p->lvupStats[0] += val;
            return;
        case 0x83:
            p->lvupStats[1] += val;
            return;
        case 0x84:
            p->lvupStats[2] += val;
            return;
        case 0x85:
            p->lvupStats[3] += val;
            return;
        case 0x86:
            p->lvupStats[4] += val;
            return;
        case 0x87:
            p->lvupStats[5] += val;
            return;
        }
    } else if (type < 0x92) {
        switch (type) {
        case 0x88:
            p->atkRef += val;
            return;
        case 0x89:
            p->startTransp += val;
            return;
        case 0x8A:
            p->startSleep += val;
            return;
        case 0x8B:
            p->startPara += val;
            return;
        case 0x8C:
            p->status2 |= 8;
            return;
        case 0x8D:
            p->status2 |= 2;
            return;
        case 0x8E:
            p->status2 |= 0x100;
            return;
        case 0x8F:
            p->status2 |= 0x200;
            return;
        case 0x90:
            p->status2 |= 0x400;
            return;
        case 0x91:
            p->status2 |= 0x1000;
            return;
        }
    } else if (type < 0x97) {
    } else if (type < 0x9F) {
        if (ptyApplyingEquip == 0) {
            p->magicRate[type - 0x96] += val;
        } else {
            p->magicPow[type - 0x96] += val;
        }
    } else if (type < 0xAD) {
        switch (type) {
        case 0x9F:
            {
                s32 i;
                if (ptyApplyingEquip == 0) {
                    for (i = 1; i < 9; i++) {
                        p->magicRate[i] += val;
                    }
                } else {
                    for (i = 1; i < 9; i++) {
                        p->magicPow[i] += val;
                    }
                }
            }
            return;
        case 0xA0:
            {
                s32 i;
                for (i = 0; i < 12; i++) {
                    p->regSta[i] += val;
                }
            }
            return;
        case 0xA1:
            p->bitCsf |= 0x2000000;
            return;
        case 0xA2:
            {
                s32 i;
                for (i = 0; i < 13; i++) {
                    p->killer[i] += val;
                }
            }
            return;
        case 0xA3:
            p->stats[0] += val;
            p->stats[1] += val;
            return;
        case 0xA4:
            p->stats[2] += val;
            p->stats[3] += val;
            return;
        case 0xA5:
            p->stats[4] += val;
            p->stats[5] += val;
            return;
        case 0xA6:
            p->startIpOfs += val * 0.0001f;
            return;
        case 0xA7:
            p->bitCsf |= 0x1000000;
            return;
        case 0xA8:
            p->getGoldRate += val;
            return;
        case 0xA9:
            p->mcAwayDec += val;
            return;
        case 0xAA:
            p->comboUp += val;
            return;
        case 0xAB:
            p->bitCsf |= 0x4000000;
            return;
        case 0xAC:
            p->antiHalf += val;
            return;
        }
    } else if (type < 0xAE) {
    } else if (type < 0xAF) {
        p->lvupMhp += val;
        p->lvupStats[0] += val;
        p->lvupStats[1] += val;
        p->lvupStats[2] += val;
        p->lvupStats[3] += val;
        p->lvupStats[4] += val;
        p->lvupStats[5] += val;
    } else if (type < 0xB2) {
        switch (type) {
        case 0xAF:
            p->exeGood += val;
            return;
        case 0xB0:
            p->atkDown += val;
            return;
        case 0xB1:
            p->defDown += val;
            return;
        }
    } else if (type < 0xB5) {
    } else if (type < 0xBA) {
        switch (type) {
        case 0xB5:
            p->cmdDisable |= 1;
            return;
        case 0xB6:
            p->cmdDisable |= 0x21;
            return;
        case 0xB7:
            p->cmdDisable |= 4;
            return;
        case 0xB8:
            p->cmdDisable |= 2;
            return;
        case 0xB9:
            p->cmdDisable |= 0x17;
            p->status2 |= 0x600;
            return;
        }
    } else if (type < 0xBE) {
    } else if (type < 0xC7) {
        p->regMgcAttr[type - 0xBE] += val;
    }
}
