#include "common.h"
#include "protos/battle.h"
#include "sys.h"
#include "battle/n29.h"
#include "party.h"

extern u8 menuCursorSprite[];
extern u8 menuNumGlyphSlash[];
extern u8 btlCmdListNumCur[];
extern u8 btlCmdListNumMax[];
extern u8 btlCmdListLeftSprites[];
extern u8 btlCmdListRightSprites[];
extern u8 btlCmdListTitleSprites[];
extern u8 btlStatLabelMp[];
extern u8 btlStatLabelSp[];
extern u8 btlCmdListSwapArrowSprite[];
extern u8 btlCmdListMultiCastSprite[];
extern u8 btlCmdListComboNgSprite[];

f32 VuSinAngle(s32);
void VuSinCosAngle(f32 *, f32 *, s32);

void BtlCmdListDraw(void) {
    BtlCmdList *p = &btlCmdList;
    s8 *f8;
    s32 t;
    s32 i;
    s32 j;
    PtyMove *eff;
    s32 k;
    f32 f;
    BtlCmdListEntry *e;
    PtyManaEgg *w;
    PtyUnit *u;
    s16 v;
    f32 va;
    f32 vb;
    BtlUnit *c;

    if (p->unit == NULL) {
        return;
    }
    t = p->state & 0xF0;
    if (t == 0xF0) {
        return;
    }
    if (t != 0x30 && t != 0x40) {
        f8 = btlCmdListPageShown;
        GphTexBind(0xEE);
        SprBegin(135.0f, p->titleSlide + 106.0f);
        sprState.col[0].a = p->unk23;
        SprDrawRectList(btlCmdListTitleSprites);
        sprState.oy = 134.0f;
        sprState.ox = p->rightSlide + 431.0f;
        SprDrawRectList(btlCmdListRightSprites);
        sprState.ox = 31.0f;
        sprState.oy = p->leftSlide + 98.0f;
        SprDrawRectList(btlCmdListLeftSprites);
        SprFlush();
        SprBegin(320.0f, p->titleSlide + 122.0f);
        sprState.col[0].a = p->unk23;
        MenuTextSetAddColor(0x10, 0x18, 0x18);
        if (f8[p->side] != 0) {
            TxtGlyphBufUpload(&btlCmdListGlyphBufs[p->side]);
            MenuTextSetOffset(0, 4);
            MenuTextDraw(&p->titles[p->side]);
        } else {
            s8 o = p->side ^ 1;
            if (f8[o] != 0) {
                TxtGlyphBufUpload(&btlCmdListGlyphBufs[o]);
                MenuTextSetOffset(0, 4);
                MenuTextDraw(&p->titles[(s8)(p->side ^ 1)]);
            }
        }
        SprFlush();
        sprState.oy = 128.0f;
        sprState.ox = p->rightSlide + 539.0f;
        if (p->mode == 2) {
            w = &ptyPartyData->eggs[p->rowState[p->row]];
            GphTexBind(0xEB);
            BtlHudSetSpColor(w->mp, w->maxMp);
            MenuNumberDraw(btlCmdListNumCur, w->mp);
            sprState.col[0].b = 0x80;
            sprState.col[0].g = 0x80;
            sprState.col[0].r = 0x80;
            MenuNumberDraw(btlCmdListNumMax, w->maxMp);
            SprDrawRect(menuNumGlyphSlash, 0, 0);
            SprFlush();
            GphTexBind(0xED);
            SprDrawRect(btlStatLabelMp, -0x50, 0);
            SprFlush();
            GphTexBind(0xEC);
            if (p->count >= 2 && p->alive != 0) {
                MenuLrArrowsDraw();
                SprFlush();
            }
        } else if (p->mode == 3) {
            GphTexBind(0xEB);
            BtlHudSetSpColor(p->unit->params->sp, p->unit->params->maxSp);
            MenuNumberDraw(btlCmdListNumCur, p->unit->params->sp);
            sprState.col[0].b = 0x80;
            sprState.col[0].g = 0x80;
            sprState.col[0].r = 0x80;
            MenuNumberDraw(btlCmdListNumMax, p->unit->params->maxSp);
            SprDrawRect(menuNumGlyphSlash, 0, 0);
            SprFlush();
            GphTexBind(0xED);
            SprDrawRect(btlStatLabelSp, -0x50, 0);
            SprFlush();
            GphTexBind(0xEC);
            if (p->count >= 2) {
                MenuLrArrowsDraw();
                SprFlush();
            }
        } else {
            if (p->count >= 2) {
                GphTexBind(0xEC);
                MenuLrArrowsDraw();
                SprFlush();
            }
        }
        SprBegin(0.0f, 0.0f);
        sprState.col[0].a = p->unk23;
        GphTexBind(0xF0);
        sprState.ox = 320.0f - (f32)(p->count - 1) * 40.0f * 0.5f;
        sprState.oy = p->leftSlide + 114.0f;
        for (i = 0; i < p->count; i++) {
            if (p->row != i) {
                if (p->rowIcon[i] != 0) {
                    if (p->rowState[i] < -1) {
                        sprState.col[0].b = 0x20;
                        sprState.col[0].g = 0x20;
                        sprState.col[0].r = 0x20;
                        SprDrawRect((SprRect *)&menuItemIcons[p->rowIcon[i]], -12, -24);
                        sprState.col[0].b = 0x80;
                        sprState.col[0].g = 0x80;
                        sprState.col[0].r = 0x80;
                    } else {
                        SprDrawRect((SprRect *)&menuItemIcons[p->rowIcon[i]], -12, -24);
                    }
                }
            }
            sprState.ox += 40.0f;
        }
        f = (VuSinAngle(btlTickCount * 0x666) + 1.0f) * 0.25f;
        sprState.sx = f + 0.75f + 0.5f;
        sprState.sy = 1.25f - f + 0.5f;
        if (f < 0.25f) {
            sprState.oy -= (0.25f - f) * 4.0f * 8.0f;
        }
        sprState.ox = 320.0f - (f32)(p->count - 1) * 40.0f * 0.5f + (f32)p->row * 40.0f;
        SprDrawRect((SprRect *)&menuItemIcons[p->rowIcon[p->row]], -12, -24);
        sprState.sx = 1.0f;
        sprState.sy = 1.0f;
        SprFlush();
        GphTexBind(0xEC);
        sprState.col[0].a = 0x80;
        if (p->col >= 0) {
            if (p->entries[p->side][p->col].playerPage != 0 && (p->entries[p->side][p->col].flags & 4)) {
                sprState.oy = p->entries[p->side][p->col].y;
                sprState.ox = p->entries[p->side][p->col].x;
                sprState.col[0].a = p->entries[p->side][p->col].alpha * 128.0f;
                MenuButtonPadDraw(0x2000, -0x44, -0x1C);
                SprDrawRect(btlCmdListMultiCastSprite, -0x50, 0);
                SprFlush();
            }
        }
        if (p->mode == 3) {
            e = &p->entries[p->side][p->col];
            if (e->flags != 0) {
                v = ptyPartyData->moveLevels[(p->unit->params->id - 0x1001) * 5 + e->cell->id].moveId;
                if (v >= 0xDD) {
                    eff = &ptyMoveTable[v];
                    SprBegin(0.0f, 20.0f);
                    if (!(btlTickCount & 0x10)) {
                        for (j = 0; j < 4; j++) {
                            if (eff->members[j] > 0) {
                                c = btlUnits;
                                for (k = 0; k < 4; k++, c++) {
                                    u = c->params;
                                    if (u->id - 0x1000 == eff->members[j] &&
                                        (((btlCmdListComboNg >> k) & 1) || u->sp < e->cell->value)) {
                                        sprState.ox = (f32)k * 160.0f + 8.0f + 10.0f;
                                        SprDrawRect(btlCmdListComboNgSprite, 0, 0);
                                    }
                                }
                            }
                        }
                    }
                    sprState.angle = 0x1200;
                    sprState.flags |= 1;
                    sprState.col[0].r = sprState.col[0].g = VuSinAngle(btlTickCount << 13) * 48.0f + 159.0f;
                    VuSinCosAngle(&va, &vb, btlTickCount << 10);
                    va *= 5.0f;
                    sprState.oy = vb * 5.0f + 56.0f;
                    for (j = 0; j < 4; j++) {
                        if (eff->members[j] > 0) {
                            c = btlUnits;
                            for (k = 0; k < 4; k++, c++) {
                                if (c->state > 0 && c->params->id - 0x1000 == eff->members[j]) {
                                    sprState.ox = (f32)k * 160.0f + 8.0f + 28.0f + va;
                                    SprDrawRect(menuCursorSprite, 0, 0);
                                }
                            }
                        }
                    }
                    SprFlush();
                }
            }
        }
    }
    if (f8[(s8)(p->side ^ 1)] != 0) {
        BtlCmdListDrawPage(1);
    }
    if (f8[p->side] != 0) {
        BtlCmdListDrawPage(0);
    }
    if (btlCmdListSwapArrowAnim >= 0) {
        GphTexBind(0xF1);
        SprBegin(76.0f, (f32)(btlCmdListSwapArrowAnim & 0xF) * 0.5f + 234.0f);
        sprState.sx = 1.5f;
        sprState.sy = 0.5f;
        sprState.col[0].g = sprState.col[0].b = (btlCmdListSwapArrowAnim >> 4) * 0x30 + 0x20;
        SprDrawRect(btlCmdListSwapArrowSprite, -12, -12);
        sprState.col[0].g += 0x30;
        sprState.oy -= 8.0f;
        if (sprState.col[0].g > 0x80) {
            sprState.col[0].g = 0x20;
        }
        sprState.col[0].b = sprState.col[0].g;
        SprDrawRect(btlCmdListSwapArrowSprite, -12, -12);
        sprState.col[0].g += 0x30;
        sprState.oy -= 8.0f;
        if (sprState.col[0].g > 0x80) {
            sprState.col[0].g = 0x20;
        }
        sprState.col[0].b = sprState.col[0].g;
        SprDrawRect(btlCmdListSwapArrowSprite, -12, -12);
        SprFlush();
    }
}
