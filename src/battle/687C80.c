/* as: ee-gcc2.9-991111-01 */
#include "common.h"
#include "protos/battle.h"
#include "sys.h"
#include "party.h"
#include "battle/n28.h"
#include "battle/n30.h"

typedef struct {
    u8 r;
    u8 g;
    u8 b;
    u8 a;
} Col;

extern SprRect menuResultElementIcons[];
extern SprRect menuLargeDigitSprites[];

extern const SprRect btlResultPortraitSprite;
extern const SprRect btlResultHeaderSprite;
extern const SprRect btlResultSprites[];
extern const SprRect btlResultBannerSprites[];
extern const u8 btlResultTopBarRects[];
extern const u8 btlResultGoldBarRects[];
extern const u8 btlResultMemberPanelRects[];
extern const u8 btlResultGoldNumFmt[];
extern const u8 btlResultGoldTotalNumFmt[];
extern const u8 btlResultExpNumFmt[];
extern const u8 btlResultNextExpNumFmt[];
extern const u8 btlResultLevelNumFmt[];
extern const u8 btlResultElementExpNumFmt[];

f32 VuSinAngle(s32);

void BtlResultDraw(void) {
    BtlResult *b;

    b = btlResult;
    if (b == NULL) {
        return;
    }
    SprBegin(0.0f, 0.0f);
    {
        BtlResultMember *e = b->members;
        s32 i;
        for (i = 0; i < 4; i++, e++) {
            if (e->active > 0) {
                sprState.ox = e->x;
                sprState.oy = e->y;
                GphTexBind((u8)btlUnits[i].params->id - 0x67);
                SprDrawRect(&btlResultPortraitSprite, 0x118, 0);
                SprFlush();
            }
        }
    }
    GphTexBind(0x99);
    sprState.ox = b->header.slideX;
    sprState.oy = 0.0f;
    SprDrawRect(&btlResultHeaderSprite, 0x18, 0x50);
    sprState.ox = 0.0f;
    sprState.oy = b->header.slideY;
    SprDrawRectList(btlResultTopBarRects);
    sprState.ox = b->slideX;
    sprState.oy = 0.0f;
    SprDrawRectList(btlResultGoldBarRects);
    sprState.ox = b->slideX2;
    sprState.oy = 64.0f;
    SprDrawRectList(btlResultGoldBarRects);
    {
        BtlResultMember *e = b->members;
        s32 i;
        for (i = 0; i < 4; i++, e++) {
            if (e->active > 0) {
                sprState.ox = e->x;
                sprState.oy = e->y;
                SprDrawRectList(btlResultMemberPanelRects);
            }
        }
    }
    SprFlush();

    if (b->state >= 0x50) {
        BtlResultMember *e;
        s32 i;
        GphTexBind(0xF1);
        e = b->members;
        for (i = 0; i < 4; i++, e++) {
            if (e->active > 0) {
                s32 j;
                s16 x;
                s16 y;
                f32 t;
                SprRect *r2;
                sprState.ox = e->x;
                sprState.oy = e->y;
                x = 0x20;
                y = 0x2E;
                for (j = 0; j < 4; j++, x += 0x4C) {
                    if (e->skillUps[j] == 0 && b->iconTimer > 0) {
                        if (b->iconTimer >= 16) {
                            continue;
                        }
                        t = 1.0f - VuSinAngle(b->iconTimer << 10);
                        sprState.col[0].a = (16 - b->iconTimer) * 8;
                        SprPushTransform((s32)((1.0f - t) * 65536.0f), x, y, t, t);
                    } else {
                        sprState.col[0].a = 0x80;
                        SprPushTransform(0, x, y, 1.0f, 1.0f);
                    }
                    SprDrawRect(&menuResultElementIcons[j], -12, -12);
                    SprPopTransform();
                }
                sprState.col[0].r = sprState.col[0].g = sprState.col[0].b = 0x80;
                sprState.col[0].a = 0x80;
                if (e->cast.state > 0) {
                    if (e->cast.timer < 16) {
                        SprPushTransform(0, 76.0f, 32.0f, 1.0f, VuSinAngle(e->cast.timer << 10));
                    } else {
                        SprPushTransform(0, 76.0f, 32.0f, 1.0f, 1.0f);
                    }
                    r2 = &menuSkillTypeIcons[ptySkillTable[e->cast.id].icon];
                    SprDrawRect(r2, -r2->w, -r2->h);
                    SprPopTransform();
                }
            }
        }
        SprFlush();
    }

    TxtGlyphBufUpload((s32 *)&btlResultGlyphBuf);
    {
        BtlResultMember *e = b->members;
        s32 i;
        for (i = 0; i < 4; i++, e++) {
            if (e->active > 0) {
                sprState.ox = e->x;
                sprState.oy = e->y;
                MenuTextSetAddColor(0x30, 0x10, 0x10);
                MenuTextDraw(&e->nameText);
                if (e->cast.state > 0) {
                    MenuTextSetAddColor(0, 0x38, 0x18);
                    if (e->cast.timer < 16) {
                        SprPushTransform(0, 80.0f, 32.0f, 1.0f, VuSinAngle(e->cast.timer << 10));
                    } else {
                        SprPushTransform(0, 80.0f, 32.0f, 1.0f, 1.0f);
                    }
                    MenuTextDraw(&e->skillText);
                    SprPopTransform();
                }
            }
        }
    }
    SprFlush();

    GphTexBind(0xEB);
    {
        BtlResultMember *e = b->members;
        s32 i;
        for (i = 0; i < 4; i++, e++) {
            if (e->active > 0) {
                sprState.ox = e->x;
                sprState.oy = e->y;
                MenuNumberDraw(btlResultLevelNumFmt, e->level);
                SprDrawRect(&menuLargeDigitSprites[16], 0x40 - menuLargeDigitSprites[16].w, 0x5C - menuLargeDigitSprites[16].h);
            }
        }
    }
    SprFlush();

    GphTexBind(0x97);
    if (0.0f < b->header.alpha) {
        sprState.ox = 131.0f;
        sprState.oy = 49.0f;
        sprState.col[0].a = (s32)(b->header.alpha * 128.0f);
        sprState.sx = b->header.scaleX;
        sprState.sy = b->header.scaleY;
        SprDrawRect(&btlResultSprites[0], -btlResultSprites[0].w / 2, -btlResultSprites[0].h / 2);
    }
    if (0.0f < b->header.alpha2) {
        sprState.ox = b->header.state == 0 ? 145.0f : 143.0f;
        sprState.oy = 101.0f;
        sprState.col[0].a = (s32)(b->header.alpha2 * 128.0f);
        sprState.sx = b->header.scaleX2;
        sprState.sy = b->header.scaleY2;
        SprDrawRect(&btlResultSprites[b->header.state + 1], -btlResultSprites[b->header.state + 1].w / 2, -btlResultSprites[b->header.state + 1].h / 2);
    }

    sprState.ox = b->slideX + 30.0f;
    sprState.sy = 1.0f;
    sprState.oy = 224.0f;
    sprState.col[0].a = 0x80;
    sprState.sx = 1.0f;
    SprDrawRect(&btlResultSprites[3], 0, 0);
    sprState.ox = b->slideX2 + 30.0f;
    sprState.oy = 288.0f;
    SprDrawRect(&btlResultSprites[4], 0, 0);
    sprState.ox = b->slideX + 170.0f;
    sprState.oy = 270.0f;
    MenuNumberDraw(btlResultGoldNumFmt, b->goldShown);
    SprDrawRect(&btlResultSprites[7], 0, -btlResultSprites[7].h);
    sprState.ox = b->slideX2 + 170.0f;
    sprState.oy = 334.0f;
    MenuNumberDraw(btlResultGoldTotalNumFmt, b->goldTotal);
    SprDrawRect(&btlResultSprites[7], 0, -btlResultSprites[7].h);

    {
        BtlResultMember *e = b->members;
        s32 i;
        for (i = 0; i < 4; i++, e++) {
            if (e->active > 0) {
                s32 k;
                s16 fl;
                sprState.ox = e->x;
                sprState.oy = e->y;
                if (0.0f < e->unkC) {
                    SprDrawRect(&btlResultSprites[8], 0xE6, 0x2A - btlResultSprites[8].h);
                    SprDrawRect(&btlResultSprites[8], 0xE6, 0x44 - btlResultSprites[8].h);
                    SprDrawRect(&btlResultSprites[5], 0x82 - btlResultSprites[5].w, 0xC);
                    SprDrawRect(&btlResultSprites[6], 0x82 - btlResultSprites[6].w, 0x28);
                    MenuNumberDraw(btlResultExpNumFmt, e->expShown);
                    MenuNumberDraw(btlResultNextExpNumFmt, e->nextExp);
                }
                if (0.0f < e->unk30) {
                    s32 j;
                    s16 x;
                    f32 s;
                    f32 t;
                    f32 u;
                    x = 0x54;
                    for (j = 0; j < 4; j++, x += 0x4C) {
                        if (e->skillUps[j] == 0 && b->iconTimer > 0) {
                            if (b->iconTimer >= 16) {
                                continue;
                            }
                            s = VuSinAngle(b->iconTimer << 10);
                            sprState.col[0].a = (16 - b->iconTimer) * 8;
                            t = 1.0f - s;
                            s = s * 8.0f + 1.0f;
                            SprPushTransform(0, x - MenuNumberWidth(btlResultElementExpNumFmt, e->elemShown[j]) * 0.5f, 56.0f, s, t);
                        } else {
                            sprState.col[0].a = 0x80;
                            SprPushTransform(0, x - MenuNumberWidth(btlResultElementExpNumFmt, e->elemShown[j]) * 0.5f, 56.0f, 1.0f, 1.0f);
                        }
                        MenuNumberDraw(btlResultElementExpNumFmt, e->elemShown[j]);
                        SprPopTransform();
                    }
                    sprState.col[0].r = sprState.col[0].g = sprState.col[0].b = 0x80;
                    sprState.col[0].a = 0x80;
                    if (e->cast.state > 0 && e->cast.timer >= 16) {
                        SprDrawRect(&btlResultSprites[9], (u16)e->skillText.width + 0x54, 0x12 - (s16)btlResultSprites[9].h / 2);
                    }
                }
                for (k = 0; k < 8; k++) {
                    if (0.0f < e->pops[k].t) {
                        sprState.ox = e->pops[k].x;
                        sprState.oy = e->pops[k].y;
                        if (e->pops[k].t < 1.0f) {
                            sprState.col[0].a = (s32)(e->pops[k].t * 128.0f);
                        }
                        SprDrawRect(&btlResultSprites[9], -btlResultSprites[9].w / 2, -btlResultSprites[9].h / 2);
                        sprState.col[0].a = 0x80;
                    }
                }
                if (btlTickCount & 0x10) {
                    fl = e->unk600;
                    if (((fl & 0x10) && 0.0f < e->unkC) || ((fl & 0x20) && 0.0f < e->unk30)) {
                        sprState.ox = e->unk602;
                        sprState.oy = e->unk604;
                        SprDrawRect(&btlResultSprites[((e->unk600 & 0x10) ? b->bonusTier : b->specialTier) + 10], -10, -btlResultSprites[10].h / 2);
                    }
                }
            }
        }
    }
    SprFlush();

    if (b->banner.state > 0) {
        BtlResultBanner *tg = &b->banner;
        GphTexBind(0x98);
        sprState.ox = tg->x;
        sprState.oy = 224.0f;
        SprDrawRect(&btlResultBannerSprites[b->banner.kind], -btlResultBannerSprites[b->banner.kind].w / 2, -btlResultBannerSprites[b->banner.kind].h / 2);
        SprFlush();
    }

    GphTexBind(0x41);
    sprState.blend = 2;
    {
        BtlResultMember *e = b->members;
        s32 i;
        for (i = 0; i < 4; i++, e++) {
            if (e->active > 0) {
                if (e->fx[0].state > 0) {
                    BtlResultFxPart *q;
                    s32 k;
                    q = e->fx[0].parts;
                    for (k = 0; k < 32; k++) {
                        sprState.ox = q->x;
                        sprState.angle = (u16)q->rot;
                        sprState.oy = q->y;
                        sprState.sy = 30.0f;
                        sprState.sx = 30.0f;
                        SprDrawRect(&ptclGlowRect, -1, -1);
                        q++;
                    }
                }
                if (e->fx[1].state > 0) {
                    BtlResultFxPart *q;
                    s32 k;
                    q = e->fx[1].parts;
                    for (k = 0; k < 32; k++) {
                        sprState.ox = q->x;
                        sprState.angle = (u16)q->rot;
                        sprState.oy = q->y;
                        sprState.sy = 30.0f;
                        sprState.sx = 30.0f;
                        SprDrawRect(&ptclGlowRect, -1, -1);
                        q++;
                    }
                }
            }
        }
    }
    SprFlush();
}
