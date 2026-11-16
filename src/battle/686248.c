#include "common.h"
#include "protos/battle.h"
#include "battle/misc.h"
#include "battle/unit.h"
#include "battle/n28.h"
#include "pad.h"
#include "sys.h"

f32 VuSinAngle(s32);

void BtlResultUpdate(void) {
    BtlResult *b;
    f32 f;

    b = btlResult;
    if (b == NULL) {
        return;
    }
    BtlResultMembersUpdate(b);
    if (padPort0Trigger & 0x20) {
        SndSePlay3D(0x7D2, 0, 0, 0);
        BtlResultCountSe(0);
        BtlResultSkip(b);
    }
    switch (b->state) {
    case 0x10:
        if (b->timer == 0x14) {
            SndSePlay3D(0x804, 0, 0, 0);
        }
        if (b->timer > 0x20) {
            b->state = 0x20;
            BtlResultCountSe(1);
        }
        break;
    case 0x20: {
        s32 i;
        BtlResultMember *p;

        for (i = 0, p = b->members; i < 4; i++, p++) {
            if (p->active && p->expShown != p->expTarget) {
                break;
            }
        }
        if (i < 4) {
            break;
        }
        b->state = (b->bonusMember >= 0) ? 0x30 : 0x40;
        BtlResultCountSe(0);
        break;
    }
    case 0x30: {
        s32 i;
        BtlResultMember *p;

        SndSePlay3D(0x805, 0, 0, 0);
        b->banner.x = 1004.0f;
        b->banner.vel = -72.0f;
        b->banner.state = 1;
        if (b->bonusMember >= 4) {
            b->banner.kind = 0;
            for (i = 0, p = b->members; i < 4; i++, p++) {
                if (p->active) {
                    p->fx[0].state = 0;
                    p->fx[0].id = 0x10;
                    p->fx[0].timer = i * -10;
                    p->fx[0].x = p->x + 230.0f + 16.0f;
                    p->fx[0].y = p->y + 36.0f - 10.0f;
                }
            }
        } else {
            b->banner.kind = 1;
            p = &b->members[b->bonusMember];
            p->fx[0].state = 0;
            p->fx[0].id = 0x10;
            p->fx[0].timer = -10;
            p->fx[0].x = p->x + 230.0f + 16.0f;
            p->fx[0].y = p->y + 36.0f - 10.0f;
        }
        b->state++;
        break;
    }
    case 0x31: {
        s32 i;
        BtlResultMember *p;

        for (i = 0, p = b->members; i < 4; i++, p++) {
            if (p->active && p->expShown != p->expGain + p->expBonus) {
                break;
            }
        }
        if (i < 4) {
            break;
        }
        BtlResultCountSe(0);
        b->state = 0x40;
        break;
    }
    case 0x50: {
        s32 n;
        BtlResultMember *p;

        SndSePlay3D(0x804, 0, 0, 0);
        p = b->members;
        for (n = 3; n >= 0; n--, p++) {
            p->unkC = 0;
            p->unk30 = 1.0f;
        }
        b->header.state = 1;
        b->header.scaleY = 1.0f;
        b->header.scaleX = 1.0f;
        b->header.alpha = 1.0f;
        b->header.timer = 0xB;
        BtlResultCountSe(1);
        b->state++;
    }
    case 0x51: {
        s32 i;
        BtlResultMember *p;

        for (i = 0, p = b->members; i < 4; i++, p++) {
            if (p->active && (p->elemShown[0] != p->elemTarget[0] || p->elemShown[1] != p->elemTarget[1] ||
                              p->elemShown[2] != p->elemTarget[2] || p->elemShown[3] != p->elemTarget[3])) {
                break;
            }
        }
        if (i < 4) {
            break;
        }
        BtlResultCountSe(0);
        if (b->specialCount > 0) {
            b->state = 0x60;
        } else {
            b->state = 0x70;
            b->iconTimer = -60;
        }
        break;
    }
    case 0x60: {
        s32 i;
        BtlResultMember *p;

        for (i = 0, p = b->members; i < 4; i++, p++) {
            if (p->active && ((btlSpecialUsedMask >> i) & 1)) {
                p->fx[1].state = 0;
                p->fx[1].id = 0x20;
                p->fx[1].timer = -10;
                p->fx[1].x = p->x + 84.0f - 18.0f;
                p->fx[1].y = p->y + 46.0f + 6.0f;
            }
        }
        SndSePlay3D(0x805, 0, 0, 0);
        b->banner.state = 1;
        b->banner.x = 1004.0f;
        b->banner.vel = -72.0f;
        b->banner.kind = (b->specialCount == 1) ? 2 : 3;
        b->state++;
        break;
    }
    case 0x61: {
        s32 i;
        BtlResultMember *p;

        for (i = 0, p = b->members; i < 4; i++, p++) {
            if (p->active && (p->elemShown[0] != p->elemExp[0] + p->elemBonus[0] || p->elemShown[1] != p->elemExp[1] + p->elemBonus[1] ||
                              p->elemShown[2] != p->elemExp[2] + p->elemBonus[2] || p->elemShown[3] != p->elemExp[3] + p->elemBonus[3])) {
                break;
            }
        }
        if (i < 4) {
            break;
        }
        BtlResultCountSe(0);
        b->state = 0x70;
        b->iconTimer = -60;
        break;
    }
    case 0x40:
    case 0x70:
        BtlResultCountSe(0);
        break;
    case 0xF0:
        if (b->timer > 0) {
            BtlResultCountSe(0);
            btlHandoffResult = 1;
            btlExitRequest = 1;
            return;
        }
        break;
    }
    if (b->slideTimer <= 16) {
        if (b->slideTimer < 16) {
            f = 1.0f - VuSinAngle(b->slideTimer << 10);
            b->slideX = f * -320.0f;
            b->slideX2 = f * -480.0f;
        } else {
            b->slideX = b->slideX2 = 0.0f;
        }
        b->slideTimer++;
    } else {
        BtlResultCountStep(&b->goldShown, b->goldGain, &b->goldStep, &b->goldDigitFrames);
        b->goldTotal = b->goldBase + b->goldShown;
    }
    if (b->banner.state > 0) {
        BtlResultBannerUpdate(b);
    }
    BtlResultHeaderUpdate(b);
    if (b->state >= 0x70 && b->iconTimer < 16) {
        b->iconTimer++;
    }
    b->timer++;
}
