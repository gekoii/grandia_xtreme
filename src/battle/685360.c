/* cc: ee-gcc2.9-991111-01 */
/* as: ee-gcc2.9-991111-01 */
#include "common.h"
#include "protos/battle.h"
#include "sys.h"
#include "party.h"
#include "battle/n28.h"

extern f32 btlResultBonusRates[];


#define ROUND(x) ((x) == (s32)(x) ? (x) : (0.0f < (x) ? (s32)(x) + 1 : -(s32)(-x) - 1))
#define CLAMP(v, hi) ((v) < 0 ? 0 : ((hi) < (v) ? (hi) : (v)))

void BtlResultStart(s32 arg0) {
    BtlResult *b;
    s32 sel;
    s32 best;
    s8 k;

    b = btlResult;
    btlPhase = 2;
    IpPanelSetMode(4, 0x12, 1);
    btlResultCountSe = -1;
    b->state = 0x10;
    b->unk8 = arg0;
    b->timer = 0;
    b->banner.state = 0;
    b->iconTimer = 0;
    b->header.state = 0;
    b->header.timer = 0;
    b->header.alpha = b->header.alpha2 = 0.0f;
    b->header.slideY = -128.0f;
    b->header.slideX = 640.0f;
    b->bonusMember = -1;
    b->header.slideTimer = 0;
    b->bonusTier = 0;
    if (btlPartyHurtLevel == 0) {
        b->bonusMember = 4;
        if (btlLevelGap >= 5) {
            b->bonusTier = 4;
        } else if (btlLevelGap >= 0) {
            b->bonusTier = 2;
        } else {
            b->bonusTier = 1;
        }
    } else {
        BtlUnit *u;
        s32 m;
        sel = -1;
        u = btlUnits;
        best = 0xF9;
        for (m = 0; m < 4; m++, u++) {
            if (u->state >= 0x40 && best < u->mvp) {
                best = u->mvp;
                sel = m;
            }
        }
        if (sel >= 0) {
            b->bonusMember = sel;
        }
        if (btlLevelGap >= 5) {
            b->bonusTier = 3;
        } else if (btlLevelGap >= 0) {
            b->bonusTier = 1;
        } else {
            b->bonusTier = 0;
        }
    }
    b->specialCount = 0;
    b->specialTier = 0;
    if (btlSpecialUsedMask != 0) {
        for (k = 0; k < 4; k++) {
            if ((btlSpecialUsedMask >> k) & 1) {
                b->specialCount++;
            }
        }
        if (btlLevelGap >= 5) {
            b->specialTier = (b->specialCount == 1) ? 3 : 4;
        } else if (btlLevelGap >= 0) {
            b->specialTier = (b->specialCount == 1) ? 1 : 2;
        } else {
            b->specialTier = (b->specialCount != 1);
        }
    }
    b->goldGain = btlGoldTotal;
    b->goldShown = 0;
    b->goldTotal = b->goldBase = ptyPartyData->gold;
    b->goldStep = 1;
    ptyPartyData->gold += b->goldGain;
    ptyPartyData->gold = CLAMP(ptyPartyData->gold, 99999999);
    b->goldDigitFrames = BtlResultCountDigitFrames(b->goldShown, b->goldGain, b->goldStep);
    b->slideTimer = 0;
    b->slideX = -320.0f;
    b->slideX2 = -480.0f;
    TxtGlyphBufInit(&btlResultGlyphBuf, sysFontImage, 0, 0xC3);
    TxtGlyphBufReset(&btlResultGlyphBuf);

    {
        SprColorQuad *p;
        TxtObj *tx;
        PtyUnit *c;
        PtySkill *e;
        PtySkillDef *d;
        PtyChar *st;
        BtlResultMember *sub;
        s32 j;
        s32 q;
        s32 r;
        s8 lv;
        s32 i;
        s32 a;
        s16 fp;
        s16 s7;
        s16 s6;
        s16 s5;
        s32 v;
        s32 n;
        s32 pts;
        s16 sk;
        BtlUnit *unit;

        sub = b->members;
        a = ROUND(btlExpTotal / (f32)btlPartyAliveCount);
        fp = ROUND(btlElementExp[0] / (f32)btlPartyAliveCount);
        s7 = ROUND(btlElementExp[1] / (f32)btlPartyAliveCount);
        s6 = ROUND(btlElementExp[2] / (f32)btlPartyAliveCount);
        s5 = ROUND(btlElementExp[3] / (f32)btlPartyAliveCount);

        unit = btlUnits;
        for (i = 0; i < 4; i++, sub++, unit++) {
            if (unit->state < 4) {
                sub->active = 0;
                continue;
            }
            tx = &sub->nameText;
            sub->active = 1;
            sub->x = 768.0f;
            sub->fx[1].state = -1;
            sub->fx[0].state = -1;
            sub->unkC = 1.0f;
            sub->unk30 = 0;
            sub->unk600 = 0;
            sub->y = i * 0x62 + 0x1E;
            p = &sub->quad;
            sub->popNext = 0;
            for (q = 0; q < 8; q++) {
                sub->pops[q].t = 0;
            }
            sub->nameText.style = 0x22;
            sub->nameText.spacing = 2;
            sub->nameText.alignY = 1.0f;
            sub->nameText.alignX = 1.0f;
            sub->nameText.scale = 1.0f;
            sub->nameText.x = 0x122;
            sub->nameText.y = 0x5C;
            TxtObjSetString(tx, &btlResultGlyphBuf, unit->params->name);
            sub->quad.w = 0x116;
            p->h = 0xA;
            p->originX = -8;
            p->originY = -0x4C;
            p->col[3].w = 0x1040F080;
            p->col[2].w = 0x1040F080;
            p->col[1].w = 0x1040F080;
            p->col[0].w = 0x1040F080;
            sub->expShown = 0;
            sub->elemShown[3] = 0;
            sub->elemShown[2] = 0;
            sub->elemShown[1] = 0;
            sub->elemShown[0] = 0;
            sub->skillUps[3] = 0;
            sub->skillUps[2] = 0;
            sub->skillUps[1] = 0;
            sub->skillUps[0] = 0;
            sub->elemBonus[3] = 0;
            sub->elemBonus[2] = 0;
            sub->elemBonus[1] = 0;
            sub->elemBonus[0] = 0;
            if (unit->state == 4) {
                sub->exp = unit->params->exp;
                sub->nextExp = unit->params->nextExp;
                sub->level = unit->params->level;
                sub->expGain = 0;
                sub->expTarget = 0;
                sub->expBonus = 0;
                sub->expStep = 0;
                sub->elemExp[3] = 0;
                sub->elemExp[2] = 0;
                sub->elemExp[1] = 0;
                sub->elemExp[0] = 0;
                sub->elemTarget[3] = 0;
                sub->elemTarget[2] = 0;
                sub->elemTarget[1] = 0;
                sub->elemTarget[0] = 0;
            } else {
                c = unit->params;
                v = ROUND(a * c->getExpRate);
                sub->expGain = v;
                sub->expTarget = v;
                if (b->bonusMember == 4 || b->bonusMember == i) {
                    sub->expBonus = ROUND(v * btlResultBonusRates[b->bonusTier]);
                } else {
                    sub->expBonus = 0;
                }
                sub->level = c->level;
                sub->exp = c->exp;
                sub->nextExp = c->nextExp;
                c->exp += sub->expGain + sub->expBonus;
                c->exp = CLAMP(c->exp, 99999999);
                PtyCharGainExp(c);
                sub->expStep = BtlResultExpStep(sub->expGain);
                sub->elemExp[0] = fp;
                sub->elemTarget[0] = fp;
                sub->elemExp[1] = s7;
                sub->elemTarget[1] = s7;
                sub->elemExp[2] = s6;
                sub->elemTarget[2] = s6;
                sub->elemExp[3] = s5;
                sub->elemTarget[3] = s5;
                if (b->specialCount > 0 && ((btlSpecialUsedMask >> i) & 1)) {
                    sub->elemBonus[0] = ROUND(fp * btlResultBonusRates[b->specialTier]);
                    sub->elemBonus[1] = ROUND(s7 * btlResultBonusRates[b->specialTier]);
                    sub->elemBonus[2] = ROUND(s6 * btlResultBonusRates[b->specialTier]);
                    sub->elemBonus[3] = ROUND(s5 * btlResultBonusRates[b->specialTier]);
                }
                st = &ptyCharData[unit->params->id - 0x1001];
                sub->gainIndex = 0;
                for (j = 0; j < 15; j++) {
                    sk = st->skills[j];
                    if (sk < 0) {
                        continue;
                    }
                    e = &ptyPartyData->skills[sk];
                    d = &ptySkillTable[e->id];
                    if (e->level >= d->maxLevel) {
                        continue;
                    }
                    n = sub->elemExp[d->category - 1] + sub->elemBonus[d->category - 1];
                    pts = e->exp + n;
                    while (pts >= d->expNeeded[e->level]) {
                        pts -= d->expNeeded[e->level];
                        sub->gains[sub->gainIndex].value = n - pts;
                        if (sub->gains[sub->gainIndex].value <= 0) {
                            sub->gains[sub->gainIndex].value = 1;
                        }
                        sub->gains[sub->gainIndex].id = e->id;
                        sub->gainIndex++;
                        if (++e->level >= d->maxLevel) {
                            pts = 0;
                            break;
                        }
                    }
                    e->exp = pts;
                    sub->skillUps[d->category - 1]++;
                }
                sub->gains[sub->gainIndex].value = 0;
            }
            sub->elemStep = 1;
            for (r = 0; r < 4; r++) {
                if (sub->elemExp[r] >= 100) {
                    sub->elemStep = 3;
                    break;
                }
            }
            TxtGlyphBufInit(&sub->glyphBuf, sysFontImage, i * 16 + 0xC4, i * 16 + 0xD3);
            sub->skillText.alignX = 0;
            sub->skillText.alignY = 1.0f;
            sub->skillText.x = 0;
            sub->skillText.y = 0;
            sub->skillText.scale = 1.0f;
            sub->skillText.spacing = sub->skillText.style = 2;
            sub->cast.state = -1;
            sub->gainIndex = (sub->gainIndex > 0) ? 0 : -1;
        }
    }
}
