#include "common.h"
#include "protos/field.h"
#include "grxsys.h"
#include "pad.h"
#include "save.h"
#include "field/menu.h"
#include "field/n23.h"
#include "field/n24.h"

FldMenuQuad *FldMenuWinGetSprBuf(u8, s32);
s32 FldMenuWinTextAdd(u8, u8, s32, s32, s32, s32, s32, s32, s32, s32);

s32 FldMenuEquipUpdate(void) {
    f32 x;
    f32 y;
    s32 k;
    u32 cnt;
    s32 n;
    s32 m;
    s32 id;
    s32 i;
    s32 v;
    s32 nv;
    s32 top;
    s32 r;
    u32 a;
    s32 yy;
    u8 *p;
    FldMenuQuad *s2;
    FldMenuQuad *s3;
    s32 idx;
    s32 up;
    s32 dn;
    u16 ici;

    m = 0;
    k = 0;
    id = sysSaveData.party[fldMenuEquip.chr] - 1;
    cnt = ItemEquipListBuild(fldMenuEquip.slot, id);
    switch (fldMenuEquip.state) {
    case 0:
        if (ChrPartyCount() >= 2) {
            FldMenuLrArrowsSetAt(1, 320.0f, 240.0f, 290.0f);
        }
        if (fldMenuEquip.chr != fldMenuEquip.next) {
            if (fldMenuEquip.dir != 0) {
                FldMenuWinClose(fldMenuEquip.page + 10, 0, 1);
                FldMenuWinClose(fldMenuEquip.page + 8, 1, 1);
            } else {
                FldMenuWinClose(fldMenuEquip.page + 10, 1, 0);
                FldMenuWinClose(fldMenuEquip.page + 8, 0, 0);
            }
            fldMenuEquip.page ^= 1;
            if (fldMenuEquip.dir != 0) {
                FldMenuWinOpen(fldMenuEquip.page + 10, 0x82, 0xE6, 0xBE, 0xDC, 0, 0, 0x18);
                FldMenuWinOpen(fldMenuEquip.page + 8, 0x1AE, 0xF0, 0x168, 0xA8, 0, 0, 0x88000);
            } else {
                FldMenuWinOpen(fldMenuEquip.page + 10, 0x82, 0xE6, 0xBE, 0xDC, 0, 1, 0x18);
                FldMenuWinOpen(fldMenuEquip.page + 8, 0x1AE, 0xF0, 0x168, 0xA8, 0, 1, 0x88000);
            }
            FldMenuWinSetActive(fldMenuEquip.page + 8);
            fldMenuEquip.chr = fldMenuEquip.next;
            break;
        }
        if (ChrPartyCount() >= 2) {
            if (padPort0[0].trigger & 4) {
                fldMenuEquip.next = FldMenuPartyPrevSlot(fldMenuEquip.chr);
                fldMenuEquip.last = -1;
                fldMenuEquip.dir = 0;
                fldMenuEquip.help = 0;
                MenuLrArrowsFlash(1);
                SndSePlay(0x7D5, 0, 0, 0x1E, 0);
                break;
            }
            if (padPort0[0].trigger & 8) {
                fldMenuEquip.next = FldMenuPartyNextSlot(fldMenuEquip.chr);
                fldMenuEquip.dir = 1;
                fldMenuEquip.last = -1;
                fldMenuEquip.help = 0;
                MenuLrArrowsFlash(0);
                SndSePlay(0x7D5, 0, 0, 0x1E, 0);
                break;
            }
        }
        if (padPort0[0].trigger & 0x1000) {
            if (fldMenuEquip.slot != 0) {
                fldMenuEquip.slot--;
            } else {
                fldMenuEquip.slot = 5;
            }
            SndSePlay(0x7D1, 0, 0, 0x1E, 0);
        } else if (padPort0[0].trigger & 0x4000) {
            if (fldMenuEquip.slot < 5) {
                fldMenuEquip.slot++;
            } else {
                fldMenuEquip.slot = 0;
            }
            SndSePlay(0x7D1, 0, 0, 0x1E, 0);
        } else {
            if (padPort0[0].trigger & 0x80) {
                if (itemWork->chars[fldMenuEquip.chr].equip[fldMenuEquip.slot].name != 0 && fldMenuEquip.slot != 0) {
                    if (PtyEquipRemove(id, fldMenuEquip.slot) >= 0) {
                        SndSePlay(0x7D2, 0, 0, 0x1E, 0);
                    } else {
                        SndSePlay(0x7D4, 0, 0, 0x1E, 0);
                    }
                } else {
                    SndSePlay(0x7D4, 0, 0, 0x1E, 0);
                }
            }
            if (padPort0[0].trigger & 0x20) {
                if (ItemEquipListBuild(fldMenuEquip.slot, id) != 0) {
                    FldMenuWinOpen(0xE, 0x1AE, 0x8C, 0x168, 0x1E, 0, 9, 9);
                    FldMenuWinOpen(0xD, 0x1AE, 0xF5, 0x168, 0x8C, 0, 0, 0x8008);
                    FldMenuWinSetActive(0xD);
                    FldMenuWinClose(fldMenuEquip.page + 8, 0, 1);
                    fldMenuEquip.state = 1;
                    fldMenuEquip.last = -1;
                    fldMenuEquip.cursor = 0;
                    fldMenuEquip.top = 0;
                    fldMenuEquip.help = 0;
                    SndSePlay(0x7D2, 0, 0, 0x1E, 0);
                } else {
                    SndSePlay(0x7D4, 0, 0, 0x1E, 0);
                }
            } else if (padPort0[0].trigger & 0x40) {
                SndSePlay(0x7D3, 0, 0, 0x1E, 0);
                return 1;
            }
        }
        break;
    case 1:
        FldMenuLrArrowsSetAt(0, 320.0f, 240.0f, 290.0f);
        if (padPort0[0].repeat & 0x1000) {
            FldMenuListCursorUp(&fldMenuEquip.cursor, &fldMenuEquip.top, 5, 2, cnt);
        }
        if (padPort0[0].repeat & 0x4000) {
            FldMenuListCursorDown(&fldMenuEquip.cursor, &fldMenuEquip.top, 5, 2, cnt);
        }
        if (padPort0[0].trigger & 0x20) {
            SndSePlay(0x7FD, 0, 0, 0x1E, 0);
            PtyEquipSet(id, fldMenuEquip.slot, itemWork->list[fldMenuEquip.top + fldMenuEquip.cursor].id);
            FldMenuWinOpen(fldMenuEquip.page + 8, 0x1AE, 0xF0, 0x168, 0xA8, 0, 0, 0x88000);
            FldMenuWinSetActive(fldMenuEquip.page + 8);
            FldMenuWinClose(0xD, 0, 1);
            FldMenuWinClose(0xE, 0, 1);
            fldMenuEquip.help = 0;
            fldMenuEquip.state = 0;
            fldMenuEquip.last = -1;
        } else if (padPort0[0].trigger & 0x40) {
            SndSePlay(0x7D3, 0, 0, 0x1E, 0);
            FldMenuWinOpen(fldMenuEquip.page + 8, 0x1AE, 0xF0, 0x168, 0xA8, 0, 0, 0x88000);
            FldMenuWinSetActive(fldMenuEquip.page + 8);
            FldMenuWinClose(0xD, 0, 1);
            FldMenuWinClose(0xE, 0, 1);
            fldMenuEquip.help = 0;
            fldMenuEquip.state = 0;
            fldMenuEquip.last = -1;
        }
        break;
    }

    if (padPort0[0].trigger & 0x10) {
        if (fldMenuEquip.help != 0) {
            fldMenuEquip.help = 0;
        } else {
            fldMenuEquip.help = 1;
        }
        fldMenuEquip.last = -1;
    }

    n = 0x14;
    FldMenuWinTextClear(fldMenuEquip.page + 10);
    FldMenuWinTextClear(fldMenuEquip.page + 8);
    FldMenuWinTextClear(0xE);
    s3 = FldMenuWinGetSprBuf(fldMenuEquip.page + 8, 0);
    FldMenuWinSetLayer(fldMenuEquip.page + 10, 0, &fldMenuTexFaces, s3, 128.0f);
    TxtMsgGetLength(sysGrxsysPac->msg->tbl[2], id);
    FldMenuWinTextAdd(fldMenuEquip.page + 10, fldMenuEquip.page, sysGrxsysPac->msg->tbl[2], 0x32, 0x2A, 0x14, 0x14, n++, id, 2);
    s3->type = 1;
    s3->v[0].r = 0x80;
    s3->v[0].g = 0x80;
    s3->v[0].b = 0x80;
    s3->v[0].a = 0;
    s3->v[0].x = 8;
    s3->v[0].y = 8;
    s3->v[1].x = 0x3E;
    s3->v[1].y = 0x3E;
    s3->z = 0;
    if (id >= 4) {
        s3->v[0].u = (id - 4) * 0x36;
        s3->v[0].v = 0x36;
        s3->v[1].u = (id - 4) * 0x36 + 0x36;
        s3->v[1].v = 0x6C;
    } else {
        s3->v[0].u = id * 0x36;
        s3->v[0].v = 0;
        s3->v[1].u = id * 0x36 + 0x36;
        s3->v[1].v = 0x36;
    }
    s3[1].type = 0;
    FldMenuBarBuild(FldMenuWinGetQuadBuf(fldMenuEquip.page + 10), 8, 0x3A, 0xAA, 8, id + 0x14);
    for (i = 0; i < 6; i++) {
        FldMenuWinTextAdd(fldMenuEquip.page + 10, fldMenuEquip.page, sysGrxsysPac->msg->tbl[7], 4, i * 0x18 + 0x44, 0xE, 0x14, n++, i + 2, 0);
    }

    v = 0;
    s3 = FldMenuWinGetSprBuf(fldMenuEquip.page + 10, 1);
    FldMenuWinSetLayer(fldMenuEquip.page + 10, 1, &fldMenuTextures[0], s3, 128.0f);
    s2 = FldMenuWinGetSprBuf(fldMenuEquip.page + 10, 2);
    FldMenuWinSetLayer(fldMenuEquip.page + 10, 2, &fldMenuTextures[6], s2, 128.0f);
    for (i = 0; i < 6; i++) {
        switch (i) {
        case 0:
            v = itemWork->chars[fldMenuEquip.chr].stats2[0];
            break;
        case 1:
            v = itemWork->chars[fldMenuEquip.chr].stats2[1];
            break;
        case 2:
            v = itemWork->chars[fldMenuEquip.chr].stats2[2];
            break;
        case 3:
            v = itemWork->chars[fldMenuEquip.chr].stats2[3];
            break;
        case 4:
            v = itemWork->chars[fldMenuEquip.chr].stats2[4];
            break;
        case 5:
            v = itemWork->chars[fldMenuEquip.chr].stats2[5];
            break;
        }
        s3 = ((FldMenuQuad *(*)(FldMenuQuad *, s32, s32, s32, f32, f32, s32, s32))FldMenuNumberBuild)(s3, v, 0, 0x48, 1.0f, 1.0f, i * 0x18 + 0x45, 0);
        if (fldMenuEquip.state != 0) {
            nv = itemWork->list[fldMenuEquip.top + fldMenuEquip.cursor].stats[i];
            if (v != nv) {
                if (nv < v) v = 1; else v = 2;
                /* floats declared first: EABI loads arguments in declaration order */
                s3 = ((FldMenuQuad *(*)(FldMenuQuad *, s32, s32, s32, f32, f32, s32, s32, s32))FldMenuNumberTintBuild)(s3, nv, 0, 0x88, 1.0f, 1.0f, i * 0x18 + 0x45, 0, v);
                p = &fldMenuEquip.anim[i];
                (*p)++;
                if (*p >= 0x3C) {
                    *p = 0;
                }
                a = *p;
                if (a >= 0x11) {
                    a = 0;
                }
                s2->type = 1;
                s2->v[0].r = 0x80;
                s2->v[0].g = 0x80;
                s2->v[0].b = 0x80;
                s2->v[0].a = 0;
                s2->v[0].x = (a >> 1) + 0x7A;
                s2->v[0].y = i * 0x18 + 0x4B - (v - 1) * 6;
                s2->v[1].x = (a >> 1) + 0x8A - a;
                s2->v[1].y = i * 0x18 + 0x4B - (v - 1) * 6 + 0x10;
                s2->z = 0;
                if (v == 2) {
                    s2->v[0].u = 0xDC;
                    s2->v[0].v = 0xDA;
                    s2->v[1].u = 0xEC;
                    s2->v[1].v = 0xEA;
                } else {
                    s2->v[0].u = 0xEE;
                    s2->v[0].v = 0xDA;
                    s2->v[1].u = 0xFE;
                    s2->v[1].v = 0xEA;
                }
                s2++;
            }
        }
    }
    s3->type = 0;
    s2->type = 0;

    s2 = FldMenuWinGetSprBuf(fldMenuEquip.page + 10, 3);
    FldMenuWinSetLayer(fldMenuEquip.page + 10, 3, &fldMenuTexLabels2, s2, 128.0f);
    s2->v[0].r = 0x80;
    s2->v[0].g = 0x80;
    s2->v[0].b = 0x80;
    s2->v[0].a = 0x80;
    s2->v[0].x = 0x3C;
    s2->v[0].y = 8;
    s2->v[1].x = 0x5A;
    s2->v[1].y = 0x1C;
    s2->v[0].u = 0xEC;
    s2->v[0].v = 0x5C;
    s2->v[1].u = 0x100;
    s2->v[1].v = 0x6A;
    s2->type = 1;
    s2->z = 0;
    s2[1].type = 0;
    ((FldMenuQuad *(*)(FldMenuQuad *, s32, s32, s32, f32, f32, s32, s32))FldMenuNumberBuild)(s3, itemWork->chars[fldMenuEquip.chr].level, 0, 0x64, 1.0f, 0.9f, 8, 0);
    FldMenuWinTextAdd(fldMenuEquip.page + 8, fldMenuEquip.page, sysGrxsysPac->msg->tbl[7], 0xC8, 0xAA, 0x18, 0x14, n, 1, 6);
    s2 = FldMenuWinGetQuadBuf(fldMenuEquip.page + 8);
    for (i = 0; i < 6; i++) {
        FldMenuWinTextAdd(fldMenuEquip.page + 8, fldMenuEquip.page, sysGrxsysPac->msg->tbl[7], 0x12, i * 0x1A + 4, 0xE, 0x14, i + 6, i + 8, 2);
        if (i == fldMenuEquip.slot && fldMenuEquip.state == 1) {
            FldMenuWinTextAdd(0xE, fldMenuEquip.page == 0, sysGrxsysPac->msg->tbl[7], 0xA, 4, 0x14, 0x14, m++, i + 8, 2);
        }
        if (itemWork->chars[fldMenuEquip.chr].equip[i].name != 0) {
            if (i == fldMenuEquip.slot) {
                FldMenuWinTextAdd(fldMenuEquip.page + 8, fldMenuEquip.page, itemWork->chars[fldMenuEquip.chr].equip[i].name, 0x6C, i * 0x1A + 4, 0x16, 0x16, k++, -1, 8);
                if (fldMenuEquip.state == 1) {
                    FldMenuWinTextAdd(0xE, fldMenuEquip.page == 0, itemWork->chars[fldMenuEquip.chr].equip[i].name, 0x78, 2, 0x16, 0x16, m++, -1, 8);
                }
            } else {
                FldMenuWinTextAdd(fldMenuEquip.page + 8, fldMenuEquip.page, itemWork->chars[fldMenuEquip.chr].equip[i].name, 0x6C, i * 0x1A + 4, 0x14, 0x14, k++, -1, 0);
            }
        }
        if (i == 0) {
            s2 = FldMenuBarBuild(s2, 4, 0x14, 0x160, 0xC, 2);
        } else {
            s2 = FldMenuBarBuild(s2, 4, i * 0x1A + 4 + 0x10, 0x160, 0xC, 1);
        }
    }

    s3 = FldMenuWinGetSprBuf(0xD, 0);
    FldMenuWinSetLayer(0xD, 1, &fldMenuTexLabels, s3, 128.0f);
    s2 = FldMenuWinGetQuadBuf(0xD);
    FldMenuWinTextClear(0xD);
    top = fldMenuEquip.top;
    if (top != 0) { up = 1; } else { up = 0; }
    if (cnt >= 6) { if (top + 5 < cnt) { dn = 1; } else { dn = 0; } } else { dn = 0; }
    FldMenuWinSetScrollArrows(0xD, up, dn);
    if (cnt >= 6) {
        cnt = 5;
    }
    for (i = 0; i < cnt; i++, top++) {
        yy = i * 0x18;
        s3->type = 1;
        ici = itemWork->list[top].icon;
        s3->v[0].r = 0x80;
        s3->v[0].g = 0x80;
        s3->v[0].b = 0x80;
        s3->z = 0;
        s3->v[0].y = yy + 4;
        s3->v[0].x = 0x14;
        s3->v[1].x = menuItemIcons[ici].w + 0x14;
        s3->v[1].y = (yy + 4) + menuItemIcons[ici].h;
        s3->v[0].u = menuItemIcons[ici].u0;
        s3->v[0].v = menuItemIcons[ici].v0;
        s3->v[1].u = menuItemIcons[ici].u1;
        s3->v[1].v = menuItemIcons[ici].v1;
        s3->v[0].a = 0;
        s3++;
        if (fldMenuEquip.slot != 0) {
            s2 = FldMenuBarBuild(s2, 8, yy + 0x12, 0x154, 0xC, 1);
        } else {
            s2 = FldMenuBarBuild(s2, 8, yy + 0x12, 0x154, 0xC, 2);
        }
        if (fldMenuEquip.state == 1) {
            if (i == fldMenuEquip.cursor) {
                FldMenuWinTextAdd(0xD, fldMenuEquip.page == 0, itemWork->list[top].name, 0x30, i * 0x18 + 4, 0x18, 0x18, m++, -1, 8);
            } else {
                FldMenuWinTextAdd(0xD, fldMenuEquip.page == 0, itemWork->list[top].name, 0x30, i * 0x18 + 4, 0x14, 0x14, m++, -1, 0);
            }
        }
    }
    s3->type = 0;
    s2->type = 0;

    if (fldMenuEquip.state != 0) {
        FldMenuCursorSetAt(0xD, 8.0f, (f32)(u32)(fldMenuEquip.cursor * 0x18) + 16.0f);
        fldMenuEquip.helpId = (s32)itemWork->list[fldMenuEquip.top + fldMenuEquip.cursor].def.item;
    } else {
        FldMenuCursorSetAt(fldMenuEquip.page + 8, 8.0f, (f32)(fldMenuEquip.slot * 0x1A) + 16.0f);
        fldMenuEquip.helpId = itemWork->chars[fldMenuEquip.chr].equip[fldMenuEquip.slot].desc;
    }
    if (fldMenuEquip.slot != 0) {
        fldMenuEquip.helpType = 5;
    } else {
        fldMenuEquip.helpType = 4;
    }
    if (fldMenuEquip.state == 0) {
        r = FldMenuWinGetState(fldMenuEquip.page + 10);
        if (r != 4 || FldMenuWinGetState(fldMenuEquip.page + 8) != r) {
            fldMenuEquip.helpId = 0;
            fldMenuEquip.last = -1;
        }
    } else {
        r = FldMenuWinGetState(fldMenuEquip.page + 10);
        if (r != 4 || FldMenuWinGetState(0xD) != r) {
            fldMenuEquip.helpId = 0;
            fldMenuEquip.last = -1;
        }
    }
    FldMenuCursorGetPos(&x, &y);
    if (fldMenuEquip.helpId != 0) {
        if (FldMenuWinGetState(fldMenuEquip.page + 8) == 4 || FldMenuWinGetState(0xD) == 4) {
            if (fldMenuEquip.state == 0) {
                idx = fldMenuEquip.slot;
            } else {
                idx = fldMenuEquip.top + fldMenuEquip.cursor;
            }
            if (fldMenuEquip.last != idx) {
                fldMenuEquip.last = idx;
                FldMenuCursorGetPos(&x, &y);
                if (fldMenuEquip.help != 0) {
                    MenuHelpOpen(0, (s8)fldMenuEquip.helpType, fldMenuEquip.helpId, x - 48.0f, y + 16.0f);
                } else {
                    MenuHelpOpen(1, (s8)fldMenuEquip.helpType, fldMenuEquip.helpId, 32.0f, 348.0f);
                }
            }
        }
    } else {
        fldMenuEquip.last = -1;
        MenuHelpClose();
    }
    return 6;
}
