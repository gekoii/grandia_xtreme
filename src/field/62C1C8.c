#include "common.h"
#include "protos/field.h"
#include "grxsys.h"
#include "field/menu.h"
#include "field/n23.h"
#include "field/n24.h"

FldMenuQuad *FldMenuWinGetSprBuf(u8, s32);
s32 FldMenuWinTextAdd(u8, u8, s32, s32, s32, s32, s32, s32, s32, s32);

void FldMenuPartyOrderSlotsBuild(void) {
    s32 a;
    s32 b;
    s32 i;
    s32 j;
    s32 lv;
    s32 w;
    s32 x;
    s32 r;
    u8 slot;
    FldMenuQuad *s;
    FldMenuQuad *t;

    for (i = 0; i < 4; i++) {
        if (fldMenuPartyOrder.slots[i] != 0) {
            lv = fldMenuPartyOrder.slots[i];
            slot = i + 9;
            lv--;
            FldMenuWinTextClear(slot);
            w = TxtMsgGetWidth(sysGrxsysPac->msg->tbl[2], lv, 0x18);
            FldMenuWinTextAdd(slot, 0, sysGrxsysPac->msg->tbl[2], 0x10C - w / 2, -4, 0x18, 0x14, i + 4, lv, 1);
            s = FldMenuWinGetSprBuf(slot, 1);
            FldMenuWinSetLayer(slot, 0, &fldMenuTextures[2], s, 128.0f);
            t = FldMenuWinGetSprBuf(slot, 2);
            FldMenuWinSetLayer(slot, 1, &fldMenuTextures[0], t, 128.0f);
            t = FldMenuNumberBuild(t, PtyCharCalcLevel(lv), 0, 0x80, 0x22, 1.0f, 0.8f, 0);
            PtyCharCalcHp(lv, &a, &b);
            t = FldMenuHpFractionBuild(t, a, b, 0xCC, 0x22);
            PtyCharCalcSp(lv, &a, &b);
            t = FldMenuFractionBuild(t, a, b, 0x84, 0x3A);
            s->type = 1;
            s->v[0].r = 0x80;
            s->v[0].g = 0x80;
            s->v[0].b = 0x80;
            s->v[0].a = 0x80;
            s->v[0].x = 0x60;
            s->v[0].y = 0x20;
            s->v[1].x = 0x7E;
            s->v[1].y = 0x34;
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
            s->v[0].x = 0xAA;
            s->v[0].y = 0x20;
            s->v[1].x = 0xC8;
            s->v[1].y = 0x34;
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
            s->v[0].x = 0x60;
            s->v[0].y = 0x38;
            s->v[1].x = 0x7E;
            s->v[1].y = 0x4C;
            s->z = 0;
            s->v[0].u = 0xCE;
            s->v[0].v = 0x82;
            s->v[1].u = 0xEC;
            s->v[1].v = 0x96;
            s[1].type = 0;
            t->type = 0;
            t = FldMenuWinGetSprBuf(slot, 3);
            FldMenuWinSetLayer(slot, 2, &fldMenuTextures[1], t, 128.0f);
            j = 0;
            x = 0xE0;
            for (; j < 6; j++) {
                r = itemWork->slotIcons[lv][j];
                t->type = 1;
                t->v[0].r = 0x80;
                t->v[0].g = 0x80;
                t->v[0].b = 0x80;
                t->v[0].a = 0;
                t->v[0].x = x;
                t->v[0].y = 0x38;
                t->v[1].x = x + 0x14;
                t->v[1].y = 0x4C;
                t->z = 0;
                t->v[0].u = menuItemIcons[r].u0;
                t->v[0].v = menuItemIcons[r].v0;
                t->v[1].u = menuItemIcons[r].u1;
                t->v[1].v = menuItemIcons[r].v1;
                x += 0x14;
                t++;
            }
            t->type = 0;
        }
    }
}
