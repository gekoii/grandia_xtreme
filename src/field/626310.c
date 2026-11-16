#include "common.h"
#include "protos/field.h"
#include "save.h"
#include "field/menu.h"
#include "field/n23.h"

/* The icon loop's flag store goes through a union (may alias the s16 icon reads). */
typedef union {
    s32 flag;
    s16 half;
} QuadTag;

FldMenuQuad *FldMenuWinGetSprBuf(u8, s32);

s32 FldMenuPartyPanelUpdate(u8 arg0) {
    s16 icon[2];
    s32 id;
    s32 ret;
    s32 n;
    s32 i;
    s32 j;
    s32 lv;
    s32 cur;
    s32 tgt;
    s32 d;
    FldMenuQuad *s;
    void *t;

    id = arg0;
    n = 0;
    ret = 0;
    fldMenuPartyPanel.tick ^= 1;
    for (i = 0; i < 4; i++) {
        if (sysSaveData.party[i] == 0) {
            continue;
        }
        lv = itemWork->chars[i].chr;
        lv--;
        if (fldMenuPartyPanel.hp[i] != itemWork->chars[i].hp || fldMenuPartyPanel.sp[i] != itemWork->chars[i].sp) {
            ret = 1;
        }
        if (fldMenuPartyPanel.flash[i] != 0) {
            if ((s8)(fldMenuPartyPanel.flashTimer[i] += 6) < 0) {
                fldMenuPartyPanel.flashTimer[i] = 0;
                fldMenuPartyPanel.flash[i] = 0;
            }
        }
        if (fldMenuPartyPanel.tick & 1) {
            cur = fldMenuPartyPanel.hp[i];
            tgt = itemWork->chars[i].hp;
            if (cur != tgt) {
                if (tgt < cur) {
                    if (tgt - 1000 < cur) {
                        fldMenuPartyPanel.hp[i] = cur - 1000;
                    } else if (tgt - 100 < cur) {
                        fldMenuPartyPanel.hp[i] = cur - 100;
                    } else if (tgt - 10 < cur) {
                        fldMenuPartyPanel.hp[i] = cur - 10;
                    } else {
                        fldMenuPartyPanel.hp[i] = cur - 1;
                    }
                } else {
                    d = tgt - cur;
                    if (d >= 1000) {
                        fldMenuPartyPanel.hp[i] = cur + 1000;
                    } else if (d >= 100) {
                        fldMenuPartyPanel.hp[i] = cur + 100;
                    } else if (d < 10) {
                        fldMenuPartyPanel.hp[i] = cur + 1;
                    } else {
                        fldMenuPartyPanel.hp[i] = cur + 10;
                    }
                }
                
            }
            cur = fldMenuPartyPanel.sp[i];
            tgt = itemWork->chars[i].sp;
            if (cur != tgt) {
                if (tgt < cur) {
                    if (tgt - 1000 < cur) {
                        fldMenuPartyPanel.sp[i] = cur - 1000;
                    } else if (tgt - 100 < cur) {
                        fldMenuPartyPanel.sp[i] = cur - 100;
                    } else if (tgt - 10 < cur) {
                        fldMenuPartyPanel.sp[i] = cur - 10;
                    } else {
                        fldMenuPartyPanel.sp[i] = cur - 1;
                    }
                } else {
                    d = tgt - cur;
                    if (d >= 1000) {
                        fldMenuPartyPanel.sp[i] = cur + 1000;
                    } else if (d >= 100) {
                        fldMenuPartyPanel.sp[i] = cur + 100;
                    } else if (d < 10) {
                        fldMenuPartyPanel.sp[i] = cur + 1;
                    } else {
                        fldMenuPartyPanel.sp[i] = cur + 10;
                    }
                }
                
            }
        }
        s = FldMenuWinGetSprBuf(id + i, 0);
        FldMenuWinSetLayer(id + i, 1, &fldMenuTextures[0], s, 128.0f);
        t = ((void *(*)(FldMenuQuad *, s32, s32, s32, s32, s32, f32, f32))FldMenuNumberBuild)(s, itemWork->chars[i].level, 0, 0x7C, 0x1A, 0, 1.0f, 0.8f);
        t = FldMenuHpFractionBuild(t, fldMenuPartyPanel.hp[i], itemWork->chars[i].hpMax, 0x7C, 0x32);
        FldMenuFractionBuild(t, fldMenuPartyPanel.sp[i], itemWork->chars[i].spMax, 0x7C, 0x4A);

        s = FldMenuWinGetSprBuf(id + i, 1);
        FldMenuWinSetLayer(id + i, 0, &fldMenuTextures[2], s, 128.0f);
        s->type = 1;
        s->v[0].r = 0x80;
        s->v[0].g = 0x80;
        s->v[0].b = 0x80;
        s->v[0].a = 0x80;
        s->v[0].x = 0x56;
        s->v[0].y = 0x18;
        s->v[1].x = 0x74;
        s->v[1].y = 0x2C;
        s->v[0].u = 0xEC;
        s->v[0].v = 0x5C;
        s->v[1].u = 0x100;
        s->v[1].v = 0x6A;
        s->z = 0;
        s++;
        s->type = 1;
        s->v[0].r = 0x80;
        s->v[0].g = 0x80;
        s->v[0].b = 0x80;
        s->v[0].a = 0x80;
        s->v[0].x = 0x56;
        s->v[0].y = 0x30;
        s->v[1].x = 0x74;
        s->v[1].y = 0x44;
        s->v[0].u = 0xCE;
        s->v[0].v = 0x5A;
        s->v[1].u = 0xEC;
        s->v[1].v = 0x6E;
        s->z = 0;
        s++;
        s->type = 1;
        s->v[0].r = 0x80;
        s->v[0].g = 0x80;
        s->v[0].b = 0x80;
        s->v[0].a = 0x80;
        s->v[0].x = 0x56;
        s->v[0].y = 0x48;
        s->v[1].x = 0x74;
        s->v[1].y = 0x5C;
        s->z = 0;
        s->v[0].u = 0xCE;
        s->v[0].v = 0x82;
        s->v[1].u = 0xEC;
        s->v[1].v = 0x96;
        s[1].type = 0;

        s = FldMenuWinGetSprBuf(id + i, 2);
        FldMenuWinSetLayer(id + i, 2, &fldMenuTextures[6], s, 128.0f);
        icon[0] = itemWork->chars[i].statusIcons[0];
        icon[1] = itemWork->chars[i].statusIcons[1];
        for (j = 0; j < 2; j++) {
            if (icon[j] >= 0) {
                ((QuadTag *)&s->type)->flag = 1;
                s->v[0].r = 0x80;
                s->v[0].g = 0x80;
                s->v[0].b = 0x80;
                s->v[0].a = 0x80;
                s->v[0].x = 0xA8 + j * 30;
                s->v[0].y = 0x18;
                s->v[1].x = 0xA8 + j * 30 + menuStatusIcons[icon[j]].w;
                s->v[1].y = 0x18 + menuStatusIcons[icon[j]].h;
                s->z = 0;
                s->v[0].u = menuStatusIcons[icon[j]].u0;
                s->v[0].v = menuStatusIcons[icon[j]].v0;
                s->v[1].u = menuStatusIcons[icon[j]].u1;
                s->v[1].v = menuStatusIcons[icon[j]].v1;
                s++;
            }
        }
        s->type = 0;

        if (fldMenuPartyPanel.flash[i] != 0) {
            FldMenuWinSetPortrait(id + n, i, (lv + 1) * 100 + 1, 0, 4);
            s = FldMenuWinGetSprBuf(id + i, 2);
            FldMenuWinSetLayer(id + i, 2, &fldMenuTexPopup, s, 128.0f);
            FldMenuWinSetLayerVisible(id + i, 2, 0);
            s->type = 1;
            s->v[0].r = 0x80;
            s->v[0].g = 0x80;
            s->v[0].b = 0x80;
            s->v[0].a = 0x80 - fldMenuPartyPanel.flashTimer[i];
            s->v[0].x = 0x10;
            s->v[0].y = 0x40 - (fldMenuPartyPanel.flashTimer[i] >> 1);
            s->v[1].x = 0x46;
            s->v[1].y = 0x55 - (fldMenuPartyPanel.flashTimer[i] >> 1);
            s->z = 0;
            s->v[0].u = 0;
            s->v[0].v = 0x21;
            s->v[1].u = 0x36;
            s->v[1].v = 0x36;
            s[1].type = 0;
        }
        n++;
    }
    return ret;
}
