/* cc: ee-gcc2.9-991111-01 */
/* as: ee-gcc2.9-991111-01 */
#include "common.h"
#include "protos/battle.h"
#include "battle/n29.h"
#include "party.h"
#include "sys.h"

typedef struct {
    u8 pad[0x18];
} Obj18;

extern SprRect btlUnitInfoSprites[];
extern Obj18 btlUnitInfoNumHp;
extern Obj18 btlUnitInfoNumHpMax;
extern Obj18 btlUnitInfoNumSp;
extern Obj18 btlUnitInfoNumSpMax;
extern Obj18 btlUnitInfoNumGauge[];
extern Obj18 btlUnitInfoNumLevel;
extern SprRect menuIconEggSlotEmpty;
extern SprRect menuNumGlyphSlash;
extern SprRect menuNumGlyphLevel;

void GsTexUpload(s32);
f32 VuSinAngle(s32);

void BtlUnitInfoDraw(void) {
    BtlUnitInfo *h;
    SprRect *r;
    SprRect *r2;
    PtyUnit *st;
    BtlChrData *eq;
    s32 j;
    s16 y;
    s32 t;
    s8 cnt;
    f32 d;
    f32 f;
    s32 fl;
    SprColorQuad *sp;
    MenuFrame *fd;
    u16 w;
    s32 i;
    s32 i1;
    s32 i2;
    s32 n;
    s32 n1;
    PtyEnemyParams *o;
    BtlChrData *e2;
    s8 *p1;
    f32 sc;
    s16 x;
    s16 x1;
    s16 x2;

    h = &btlUnitInfo;
    fd = &btlUnitInfoFrame;
    if (btlUnitInfo.unit == NULL) {
        return;
    }
    if (h->state < 0x10) {
        return;
    }
    if (h->state >= 0x20) {
        return;
    }
    MenuFrameDraw(fd);
    if (btlUnitInfo.unit->id < 4) {
        t = ChrPortraitGetTex(btlUnitInfo.unit->def->slot);
        GsTexUpload(t);
        GsTexPacketSend(t);
        SprBegin(h->frame.x, 0.0f);
        sprState.col[0].r = 0x40;
        sprState.col[0].g = 0x40;
        sprState.col[0].b = 0x40;
        sprState.col[0].a = fd->alpha * 96.0f;
        sprState.oy = fd->y;
        sprState.angle = fd->angle;
        sprState.flags |= 1;
        {
            SprRect *rr = &btlUnitInfoSprites[18];
            s32 tw = (u16)rr[1].w - 0x80;
            SprDrawRect(rr, fd->w + tw, 0);
        }
        sprState.flags |= 4;
        sprState.col[1].r = sprState.col[2].r = sprState.col[3].r = sprState.col[0].r;
        sprState.col[1].g = sprState.col[2].g = sprState.col[3].g = sprState.col[0].g;
        sprState.col[1].b = sprState.col[2].b = sprState.col[3].b = sprState.col[0].b;
        sprState.col[2].a = sprState.col[3].a = sprState.col[0].a;
        sprState.col[0].a = sprState.col[1].a = 0;
        SprDrawRect(&btlUnitInfoSprites[19], fd->w - 0x80, 0);
    }
    SprFlush();
    SprBegin(h->frame.x, 0.0f);
    sprState.oy = fd->y;
    sprState.col[0].a = fd->alpha * 128.0f;
    sprState.angle = fd->angle;
    if (h->detail != 0) {
        GphTexBind(0xED);
        SprDrawRect(&btlUnitInfoSprites[10], 0x28, 2);
        SprDrawRect(&btlUnitInfoSprites[11], 0xC0, 2);
        SprFlush();
        TxtGlyphBufUpload(&btlUnitInfoSkillGlyphBuf);
        for (i = 0; i < h->skillCount; i++) {
            MenuTextDraw(&btlUnitInfoSkillTexts[i]);
        }
        SprFlush();
        y = 0x12;
        GphTexBind(0xF1);
        o = &ptyBattleParams[h->unit->id - 4];
        for (j = 0; j < 6; j++) {
            if (o->skillLevels[j] > 0) {
                x = 0xC0;
                for (n = 0; n < o->skillLevels[j]; n++) {
                    SprDrawRect(&btlUnitInfoSprites[15], x, y);
                    x += 0xC;
                }
                y += 0x12;
            }
        }
        if (h->skillCount > 0) {
            SprFlush();
            GphTexBind(0xED);
            SprDrawRect(&btlUnitInfoSprites[13], 0x132 - btlUnitInfoSprites[13].w, -0xE);
            SprFlush();
            GphTexBind(0xEC);
        }
    } else {
        r = btlUnitInfoSprites;
        GphTexBind(0xED);
        SprDrawRect(&r[0], 8, 0x2A);
        SprDrawRect(&r[1], 0xB, 0x53);
        if (btlUnitInfo.unit->state != 0x81) {
            SprDrawRect(&r[2], 0xC3, 0x53);
            SprDrawRect(&r[3], 0x38, 0x54);
            w = r[3].w;
            SprDrawRect(&r[4], w + 0x38, 0x54);
            w = w + (s16)(r[4].w + 0x38);
            SprDrawRect(&r[5], w, 0x54);
            SprDrawRect(&r[6], 0xF0, 0x57);
            SprDrawRect(&r[7], r[6].w + 0xF0, 0x57);
        }
        if (btlUnitInfo.unit->id >= 4) {
            s8 *q;
            s32 k;
            if (btlUnitInfo.unit->treasure > 0) {
                SprDrawRect(&r[8], 7, 32.0f - (f32)(r[8].h / 2));
            }
            cnt = 0;
            q = btlUnitInfo.unit->def->data->expSklKind;
            for (k = 0; k < 2; k++) {
                if (q[k] != 0) {
                    cnt++;
                }
            }
            if (cnt > 0) {
                SprDrawRect(&btlUnitInfoSprites[9], 256.0f - (f32)btlUnitInfoSprites[9].w - 4.0f + (f32)((2 - cnt) * 0x28), 0);
            }
        }
        if (h->skillCount > 0) {
            SprDrawRect(&btlUnitInfoSprites[12], 0x12E - btlUnitInfoSprites[12].w, -0xE);
        }
        SprFlush();
        GphTexBind(0xED);
        r2 = btlStatLabelSprites;
        SprDrawRect(&r2[0], 7, 3);
        if (h->unit->id < 4) {
            SprDrawRect(&r2[2], 7, 0x17);
            if (BtlUnitCountManaEggs(h->unit) != 0) {
                SprDrawRect(&r2[1], 0xF0 - r2[1].w / 2, 2);
            }
        }
        SprFlush();
        if (h->unit->id >= 4) {
            e2 = h->unit->def->data;
            GphTexBind(0xF1);
            for (i1 = 0; i1 < 2; i1++) {
                if (e2->expSklKind[i1] != 0) {
                    SprDrawRect(&menuSkillTypeIcons[e2->expSklKind[i1]], (f32)((1 - i1) * 0x28) + 228.0f, 0xE);
                }
            }
            SprFlush();
        }
        GphTexBind(0xF0);
        if (h->unit->id < 4) {
            if (BtlUnitCountManaEggs(h->unit) != 0) {
                s16 *ids;
                s16 id;
                s32 k;
                s32 km;
                k = h->unit->params->id;
                n1 = ptyCharDefTable[k - 0x1001].eggSlots;
                ids = ptyCharData[k - 0x1001].slots;
                km = n1 - 1;
                x1 = 240.0f - (f32)(km * 0x14) * 0.5f;
                while (n1 > 0) {
                    id = *ids;
                    if ((u16)id >= 0x18) {
                        SprPushTransform(0, x1, 36.0f, 0.75f, 0.75f);
                        SprDrawRect(&menuIconEggSlotEmpty, -0xC, -0x14);
                    } else {
                        s32 c;
                        f32 fv;
                        f32 dv;
                        sc = (f32)ptyPartyData->eggs[id].mp / (f32)ptyPartyData->eggs[id].maxMp;
                        fv = (VuSinAngle((btlTickCount + n1 * 7) * 0x666) * sc + 1.0f) * 0.2f;
                        SprPushTransform(0, x1, fv < 0.2f ? 40.0f - (0.2f - fv) * 4.0f * 6.0f : 40.0f, (fv + 0.8f) * 0.75f, (1.2f - fv) * 0.75f);
                        c = sc * 128.0f;
                        sprState.col[0].b = c;
                        sprState.col[0].g = c;
                        sprState.col[0].r = c;
                        SprDrawRect((SprRect *)&menuItemIcons[ptyItemTable[ptyPartyData->eggs[id].itemId].icon], -0xC, -0x18);
                        sprState.col[0].b = 0x80;
                        sprState.col[0].g = 0x80;
                        sprState.col[0].r = 0x80;
                    }
                    ids++;
                    SprPopTransform();
                    n1--;
                    x1 += 0x14;
                }
                SprFlush();
            }
        }
        if (h->unit->treasure > 0) {
            s32 c;
            x2 = (f32)btlUnitInfoSprites[8].w + 7.0f + 4.0f;
            c = VuSinAngle(btlTickCount << 13) * 16.0f + 128.0f;
            sprState.col[0].b = c;
            sprState.col[0].g = c;
            sprState.col[0].r = c;
            for (i = 0; i < h->unit->treasure; i++) {
                SprDrawRect(&btlUnitInfoSprites[14], x2, 32.0f - (f32)(btlUnitInfoSprites[14].h / 2));
                x2 += 0x14;
            }
            SprFlush();
            sprState.col[0].b = 0x80;
            sprState.col[0].g = 0x80;
            sprState.col[0].r = 0x80;
        }
        if (h->unit->state != 0x81) {
            GphTexBind(0xEA);
            BtlUnitIconDraw(&btlUnits[h->unit->target], 0x120, 0x74, 0x3000);
            SprFlush();
        }
        GphTexBind(0xEB);
        st = h->unit->params;
        SprDrawRect(&menuNumGlyphSlash, 0x79, 0);
        BtlHudSetHpColor(h->unit->params->hp, h->unit->params->maxHp);
        MenuNumberDraw(&btlUnitInfoNumHp, st->hp);
        sprState.col[0].b = 0x80;
        sprState.col[0].g = 0x80;
        sprState.col[0].r = 0x80;
        MenuNumberDraw(&btlUnitInfoNumHpMax, st->maxHp);
        if (h->unit->id < 4) {
            SprDrawRect(&menuNumGlyphSlash, 0x79, 0x14);
            BtlHudSetSpColor(h->unit->params->sp, h->unit->params->maxSp);
            MenuNumberDraw(&btlUnitInfoNumSp, st->sp);
            sprState.col[0].b = 0x80;
            sprState.col[0].g = 0x80;
            sprState.col[0].r = 0x80;
            MenuNumberDraw(&btlUnitInfoNumSpMax, st->maxSp);
        } else {
            eq = h->unit->def->data;
            p1 = eq->expSklKind;
            for (i2 = 0; i2 < 2; i2++) {
                if (p1[i2] != 0) {
                    f32 ox;
                    sp = &btlUnitInfoGaugeQuads[p1[i2] - 1];
                    ox = (f32)((1 - i2) * 0x28);
                    sp->h = (f32)st->expSkl[i2] / (f32)eq->gaugeMax[i2] * 48.0f;
                    sp->originY = -(50.0f - (f32)sp->h);
                    sp->originX = -(256.0f - (f32)sp->w + 4.0f + ox);
                    SprDrawColorQuad(sp, fd->alpha);
                    if (eq->gaugeMin[i2] >= st->expSkl[i2]) {
                        sprState.col[0].b = 0x40;
                        sprState.col[0].g = 0x40;
                        sprState.col[0].r = 0x40;
                    }
                    SprPushTransform(0, ox + 256.0f, 50.0f, 1.0f, 0.7f);
                    MenuNumberDraw(&btlUnitInfoNumGauge[i2], st->expSkl[i2]);
                    SprPopTransform();
                    sprState.col[0].b = 0x80;
                    sprState.col[0].g = 0x80;
                    sprState.col[0].r = 0x80;
                }
            }
        }
        SprFlush();
        GphTexBind(0xEC);
        if (h->unit->powUp & 0x101) {
            SprDrawRect(&menuStatusIcons[12], 0xA0, 0x34);
            if (btlTickCount & 0x40) {
                SprRect *sel;
                if (h->unit->powUp & 0x100) {
                    sel = &menuStatusIcons[18];
                } else {
                    sel = &menuStatusIcons[19];
                }
                SprDrawRect(sel, 0xA8, 0x3E);
            }
        }
        if (h->unit->powUp & 0x202) {
            SprDrawRect(&menuStatusIcons[13], 0xBA, 0x34);
            if (btlTickCount & 0x40) {
                SprRect *sel;
                if (h->unit->powUp & 0x200) {
                    sel = &menuStatusIcons[18];
                } else {
                    sel = &menuStatusIcons[19];
                }
                SprDrawRect(sel, 0xC2, 0x3E);
            }
        }
        if (h->unit->powUp & 0x404) {
            SprDrawRect(&menuStatusIcons[15], 0xD4, 0x34);
            if (btlTickCount & 0x40) {
                SprRect *sel;
                if (h->unit->powUp & 0x400) {
                    sel = &menuStatusIcons[18];
                } else {
                    sel = &menuStatusIcons[19];
                }
                SprDrawRect(sel, 0xDC, 0x3E);
            }
        }
        if (h->unit->powUp & 0x808) {
            SprDrawRect(&menuStatusIcons[16], 0xEE, 0x34);
            if (btlTickCount & 0x40) {
                SprRect *sel;
                if (h->unit->powUp & 0x800) {
                    sel = &menuStatusIcons[18];
                } else {
                    sel = &menuStatusIcons[19];
                }
                SprDrawRect(sel, 0xF6, 0x3E);
            }
        }
        if (h->unit->powUp & 0x1010) {
            SprDrawRect(&menuStatusIcons[17], 0x108, 0x34);
            if (btlTickCount & 0x40) {
                SprRect *sel;
                if (h->unit->powUp & 0x1000) {
                    sel = &menuStatusIcons[18];
                } else {
                    sel = &menuStatusIcons[19];
                }
                SprDrawRect(sel, 0x110, 0x3E);
            }
        }
        f = 0.0f;
        fl = st->status;
        if (fl & 0xC810) {
            if (fl & 0x8000) {
                SprDrawRect(&menuStatusIcons[11], f + 10.0f, 0x38);
            } else if (fl & 0x4000) {
                SprDrawRect(&menuStatusIcons[10], f + 10.0f, 0x38);
            } else if (fl & 0x800) {
                SprDrawRect(&menuStatusIcons[7], f + 10.0f, 0x38);
            } else if (fl & 0x10) {
                SprDrawRect(&menuStatusIcons[0], f + 10.0f, 0x38);
            }
            f += 20.0f;
        }
        if (fl & 0xE0) {
            if (fl & 0x80) {
                SprDrawRect(&menuStatusIcons[3], f + 10.0f, 0x38);
            } else if (fl & 0x40) {
                SprDrawRect(&menuStatusIcons[2], f + 10.0f, 0x38);
            } else if (fl & 0x20) {
                SprDrawRect(&menuStatusIcons[1], f + 10.0f, 0x38);
            }
            f += 20.0f;
        }
        if (fl & 0x100) {
            SprDrawRect(&menuStatusIcons[4], f + 10.0f, 0x38);
            f += 20.0f;
        }
        if (fl & 0x200) {
            SprDrawRect(&menuStatusIcons[5], f + 10.0f, 0x38);
            f += 20.0f;
        }
        if (fl & 0x400) {
            SprDrawRect(&menuStatusIcons[6], f + 10.0f, 0x38);
            f += 20.0f;
        }
        if (fl & 0x1000) {
            SprDrawRect(&menuStatusIcons[8], f + 10.0f, 0x38);
            f += 20.0f;
        }
        SprFlush();
    }
    if (h->skillCount > 0) {
        SprDrawRect(&btlUnitInfoSprites[17], (btlTickCount & 0x1F) / 4 + 0x12C, -0xE);
        SprFlush();
    }
    if (btlUnitInfoNameText.width < 0xB5) {
        sc = 1.0f;
    } else {
        sc = 180.0f / (f32)btlUnitInfoNameText.width;
    }
    sprState.oy = 0.0f;
    GphTexBind(0xEA);
    BtlUnitIconDraw(h->unit, (s16)(h->slideX + 0.0f), 0x122, 0x2000);
    SprFlush();
    GphTexBind(0xEB);
    SprPushTransform(0, (f32)btlUnitInfoNameText.width * sc + 26.0f + h->slideX, 300.0f, 1.0f, 0.8f);
    if (h->unit->id >= 4) {
        d = (f32)h->unit->params->level - btlPartyAvgLevel;
        if (d >= 4.0f) {
            sprState.col[0].g = 0x50;
            sprState.col[0].b = 0x20;
        } else if (d >= 2.0f) {
            sprState.col[0].g = 0x80;
            sprState.col[0].b = 0x30;
        } else {
            sprState.col[0].g = 0x60;
            sprState.col[0].r = 0x60;
        }
    } else {
        sprState.col[0].g = 0x60;
        sprState.col[0].r = 0x60;
    }
    MenuNumberDraw(&btlUnitInfoNumLevel, h->unit->params->level);
    SprDrawRect(&menuNumGlyphLevel, 0, -menuNumGlyphLevel.h);
    sprState.col[0].b = 0x80;
    sprState.col[0].g = 0x80;
    sprState.col[0].r = 0x80;
    SprPopTransform();
    SprFlush();
    TxtGlyphBufUpload(&btlUnitInfoGlyphBuf);
    SprPushTransform(0, h->slideX + 10.0f + 8.0f, 278.0f, sc, 1.0f);
    MenuTextDraw(&btlUnitInfoNameText);
    SprPopTransform();
    if (h->detail == 0) {
        sprState.oy = fd->y;
        MenuTextDraw(&btlUnitInfoActionText);
    }
    SprFlush();
}
