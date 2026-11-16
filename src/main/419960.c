#include "common.h"
#include "protos/main.h"
#include "party.h"
#include "main/n02.h"

/* menuHelp.text[i] addressed from the window base: (View *)((u8 *)&menuHelp + i * 0x24)
 * puts text[i].x/y/style at +0x48/+0x4A/+0x50. The match needs the 0x34 in
 * the field offsets rather than in the address. */
typedef struct {
    u8 pad0[0x48];
    s16 x;
    s16 y;
    u8 pad4C[4];
    s8 kind;
} View;

extern s8 menuHelpElemAffinity[];
extern u8 menuHelpElementLabels[];
extern u8 menuHelpStrGood[];
extern u8 menuHelpStrBad[];
extern u8 menuHelpStrImmunity[];


void MenuHelpBuildArmor(void) {
    MenuHelp *m;
    u8 *base;
    PtyItem *e;
    PtyEquipData *pt;
    s8 n;
    s8 cnt;
    s32 i;
    s8 k;
    s8 k0;
    s32 j;
    s16 x;
    s16 y1;
    s16 y2;
    s32 d;
    s32 t1;
    s32 t2;
    s32 o1;
    s32 o2;
    s32 o3;
    s32 o4;
    s32 o5;
    s16 *arr;

    m = &menuHelp;
    base = (u8 *)m;
    cnt = 0;
    e = m->data;
    pt = e->equip;
    n = 0;
    if (m->style == 1) {
        if (e->type != 10 && MenuHelpAddStats(6, 6) != 0) {
            cnt = 1;
            m->lines[0] = 4;
        }
        if (cnt < 2) {
            for (j = 1; j < 9; j++) {
                if (pt->elements[j] > 0) {
                    break;
                }
            }
            if (j < 9) {
                m->lines[cnt] = 6;
                MenuHelpAddEffectLabel(&n, 6, 6, cnt * 24 + 6);
                cnt++;
            }
        }
        if (cnt < 2) {
            if (pt->unk10 != 0) {
                m->lines[cnt] = 7;
                MenuHelpAddEffectLabel(&n, 7, 6, cnt * 24 + 6);
                cnt++;
            }
        }
        if (cnt < 2) {
            if (pt->effectType != 0) {
                if ((u8)(pt->effectType - 0x28) < 12) {
                    m->lines[cnt] = 8;
                } else {
                    m->lines[cnt] = 5;
                }
                MenuHelpAddEffectLabel(&n, m->lines[cnt], 6, cnt * 24 + 6);
                cnt++;
            }
        }
        if (cnt < 2) {
            if (e->use != NULL) {
                m->lines[cnt] = 9;
                MenuHelpAddEffectLabel(&n, 9, 6, cnt * 24 + 6);
                cnt++;
            }
        }
        if (cnt == 0) {
            if (e->type == 10) {
                if (MenuHelpAddStats(6, 6) != 0) {
                    cnt = 1;
                    m->lines[0] = 4;
                }
            }
        }
    } else {
        MenuHelpAddName(&n, 6, 6);
        m->lines[0] = 1;
        cnt = 1;
        if (MenuHelpAddStats(0x2E, 0x1E) != 0) {
            cnt = 2;
            m->lines[1] = 4;
        }
        k = 0;
        arr = pt->elements;
        for (i = 1; i < 9; i++) {
            if (arr[i] > 0) {
                menuHelpElemAffinity[k] = i;
                k++;
            }
        }
        if (k > 0) {
            x = 0x2E - m->frame.ox;
            o1 = n * 0x24;
            ((View *)(base + o1))->kind = 1;
            ((View *)(base + o1))->x = x;
            x += 0x32;
            ((View *)(base + o1))->y = cnt * 24 - ((u16)m->frame.oy - 6);
            TxtObjSetString(&base[o1 + 0x34], menuHelpGlyphBuf, menuHelpStrGood);
            n++;
            t1 = n - 1;
            y1 = ((View *)(t1 * 0x24 + base))->y;
            for (i = 0; i < k; i++) {
                o2 = n * 0x24;
                ((View *)(base + o2))->x = x;
                ((View *)(base + o2))->kind = 0x12;
                x += 0x1E;
                ((View *)(base + o2))->y = y1;
                TxtObjSetString(&base[o2 + 0x34], menuHelpGlyphBuf, menuHelpElementLabels + menuHelpElemAffinity[i]);
                n++;
            }
        }
        k0 = k;
        for (i = 1; i < 9; i++) {
            if (arr[i] < 0) {
                menuHelpElemAffinity[k] = -i;
                k++;
            }
        }
        d = k - k0;
        if (d > 0) {
            x = 0xE2 - m->frame.ox;
            if (k0 == 0) {
                x = 0x2E - m->frame.ox;
            } else if (d >= 5) {
                x = x - (d - 4) * 30;
            }
            o3 = n * 0x24;
            ((View *)(base + o3))->x = x;
            ((View *)(base + o3))->kind = 1;
            x += 0x32;
            ((View *)(base + o3))->y = cnt * 24 - ((u16)m->frame.oy - 6);
            TxtObjSetString(&base[o3 + 0x34], menuHelpGlyphBuf, menuHelpStrBad);
            n++;
            t2 = n - 1;
            y2 = ((View *)(t2 * 0x24 + base))->y;
            for (i = k0; i < k; i++) {
                o4 = n * 0x24;
                ((View *)(base + o4))->x = x;
                ((View *)(base + o4))->kind = 2;
                x += 0x1E;
                ((View *)(base + o4))->y = y2;
                TxtObjSetString(&base[o4 + 0x34], menuHelpGlyphBuf, menuHelpElementLabels - menuHelpElemAffinity[i]);
                n++;
            }
        }
        if (k > 0) {
            if (k < 8) {
                menuHelpElemAffinity[k] = 0;
            }
            m->lines[cnt] = 6;
            cnt++;
        }
        if (pt->unk10 != 0) {
            o5 = n * 0x24;
            ((View *)(base + o5))->kind = 1;
            ((View *)(base + o5))->x = 0x2E - m->frame.ox;
            ((View *)(base + o5))->y = cnt * 24 - ((u16)m->frame.oy - 6);
            TxtObjSetString(&base[o5 + 0x34], menuHelpGlyphBuf, menuHelpStrImmunity);
            m->lines[cnt] = 7;
            n++;
            cnt++;
        }
        if (cnt < 4) {
            if ((u8)(pt->effectType - 0x28) < 12) {
                MenuHelpAddDamageReduce(&n, 0x2E, cnt * 24 + 6);
                m->lines[cnt] = 8;
                cnt++;
            }
        }
        if (cnt < 4) {
            if (*(u8 *)((PtyItem *)m->data)->helpDesc != 0) {
                MenuHelpAddDesc(&n, 0x2E, cnt * 24 + 6);
                m->lines[cnt] = 2;
                cnt++;
            }
        }
        if (cnt < 4) {
            if (e->use != NULL) {
                MenuHelpAddLinkedSkill(&n, e->use, 0x2E, 0x4E);
                m->lines[cnt] = 9;
                cnt++;
            }
        }
    }
    for (; cnt < 4; cnt++) {
        m->lines[cnt] = 0;
    }
}
