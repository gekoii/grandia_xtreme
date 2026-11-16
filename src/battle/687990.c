#include "common.h"
#include "protos/battle.h"
#include "battle/unit.h"
#include "battle/n28.h"
#include "party.h"
#include "sys.h"


void BtlResultSkip(BtlResult *b) {
    BtlResultMember *sub;
    BtlUnit *u;
    s32 k;
    s32 j;
    s32 k2;
    BtlResultMember *sub2;
    f32 c;

    switch (b->state & 0xF0) {
    case 0x20:
    case 0x30:
        if ((b->state & 0xF0) != 0x20 && b->state <= 0x30) {
            break;
        }
        sub = b->members;
        u = btlUnits;
        for (k = 0; k < 4; k++, sub++, u++) {
            if ((b->state & 0xF0) == 0x20) {
                sub->expShown = sub->expGain;
                if (b->bonusMember < 0) {
                    if (sub->level != u->params->level) {
                        BtlResultLevelUpPop(sub);
                        sub->level = u->params->level;
                    }
                    sub->nextExp = u->params->nextExp;
                }
            } else {
                sub->expShown = sub->expGain + sub->expBonus;
                if (sub->level != u->params->level) {
                    BtlResultLevelUpPop(sub);
                    sub->level = u->params->level;
                }
                sub->nextExp = u->params->nextExp;
            }
        }
        if ((b->state & 0xF0) == 0x20) {
            b->state = (b->bonusMember >= 0) ? 0x30 : 0x40;
        } else {
            b->state = 0x40;
        }
        break;
    case 0x40:
        b->state = 0x50;
        break;
    case 0x60:
        if (b->state <= 0x60) {
            break;
        }
    case 0x50:
        sub2 = b->members;
        for (k2 = 0; k2 < 4; k2++, sub2++) {
            for (j = 0; j < 4; j++) {
                sub2->elemShown[j] = ((b->state & 0xF0) == 0x50) ? sub2->elemExp[j] : sub2->elemExp[j] + sub2->elemBonus[j];
            }
        }
        if ((b->state & 0xF0) == 0x50) {
            b->state = (b->specialCount > 0) ? 0x60 : 0x70;
        } else {
            b->state = 0x70;
        }
        break;
    case 0x70:
        b->goldShown = b->goldGain;
        switch (btlHandoffEndFade) {
        case 1:
            c = 0.0f;
            GphScreenFadeSetColor(c, c, c, c);
            GphScreenFadeStart(0, c, c, c, 128.0f, 30.0f);
            b->timer = -30;
            break;
        case 2:
            c = 255.0f;
            GphScreenFadeSetColor(c, c, c, 0.0f);
            GphScreenFadeStart(0, c, c, c, 128.0f, 30.0f);
            b->timer = -30;
            break;
        default:
            b->timer = -5;
            break;
        }
        b->state = 0xF0;
        break;
    }
}
