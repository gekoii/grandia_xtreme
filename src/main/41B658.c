/* cc: ee-gcc2.9-991111-01 */
/* as: ee-gcc2.9-991111-01 */
#include "common.h"
#include "protos/main.h"
#include "sys.h"
#include "party.h"
#include "main/n02.h"

/* menuHelp.text[i] addressed from the window base: (View *)((u8 *)&menuHelp + i * 0x24)
 * puts text[i].x/y/style at +0x48/+0x4A/+0x50. The match needs the 0x34 in
 * the field offsets rather than in the address. */
typedef struct {
    u8 pad0[0x48];
    s16 x;
    s16 y;
    u8 pad4C[6];
    s16 w;
} View;

extern const SprRect menuHelpTailSprites[];
extern SprRect menuHelpImmunityAllIcon;
extern SprRect menuHelpLinkIcon;
extern s8 menuHelpWeaponIcons[];
extern s8 menuHelpLinkValue;
extern s8 menuHelpElemAffinity[];
extern s8 menuHelpTargetIsFoe;
extern s8 menuHelpVsType;
extern s8 menuHelpElement;
extern s8 menuHelpStatusNo;

void MenuHelpDraw(void) {
    MenuHelp *m;
    u8 *base;
    MenuFrame *w;
    s8 n;
    s8 *items;
    s32 i;
    s32 j;
    s32 k;
    s32 c;
    s32 q;
    s32 y;
    f32 sc;
    f32 fx;

    m = &menuHelp;
    base = (u8 *)m;
    w = &m->frame;
    n = 0;
    menuHelpFrame++;
    if (m->state == 0) {
        return;
    }
    MenuFrameDraw(w);
    SprBegin(m->frame.x, w->y);
    sprState.angle = w->angle;
    sprState.sx = w->sx;
    sprState.sy = w->sy;
    sprState.col[0].a = w->alpha * 128.0f;
    if (m->style == 1) {
        SprDrawRect(&menuHelpTailSprites[2], w->w - w->ox - menuHelpTailSprites[0].w - menuHelpTailSprites[2].w - 4, w->h - w->oy);
        SprDrawRect(&menuHelpTailSprites[0], w->w - w->ox - menuHelpTailSprites[0].w - 4, w->h - w->oy - menuHelpTailSprites[0].h / 2 + menuHelpTailSprites[2].h / 2);
        SprDrawRect(&menuHelpTailSprites[1], w->w - w->ox, w->h - w->oy - menuHelpTailSprites[1].h / 2 + menuHelpTailSprites[2].h / 2);
    } else {
        SprDrawRect(&menuHelpTailSprites[0], -w->ox, -w->oy - menuHelpTailSprites[0].h - 4);
    }
    SprFlush();
    if ((m->state & 0xF0) == 0x10) {
        return;
    }
    sprState.col[0].a = m->alpha * 128.0f;
    if (m->style == 2) {
        sc = 1.0f;
        TxtGlyphBufUpload(menuHelpGlyphBuf);
        MenuTextSetAddColor(0x60, 8, 8);
        if (!(m->text[n].width <= 80.0f)) {
            sc = 80.0f / m->text[n].width;
        }
        SprPushTransform(0, 6 - m->frame.ox, 0.0f, sc, 1.0f);
        MenuTextDraw(&m->text[n]);
        SprPopTransform();
        SprSetColor(0x70, 0x70, 0x80);
        MenuTextSetAddColor(0, 0, 0x40);
        MenuTextDraw(&m->text[1]);
        SprSetColor(0x80, 0x80, 0x80);
        MenuTextSetAddColor(0x10, 0x38, 0x58);
        sc = 1.0f;
        if (!(m->text[2].width <= 364.0f)) {
            sc = 364.0f / m->text[2].width;
        }
        SprPushTransform(0, 36.0 - m->frame.ox, 0.0f, sc, 1.0f);
        MenuTextDraw(&m->text[2]);
        SprPopTransform();
        SprFlush();
        return;
    }
    GphTexBind(0xF1);
    if (m->style == 1) {
        items = m->lines;
        for (i = 0; i < 4; i++) {
            switch (items[i]) {
            case 6:
                if (m->kind == 4 && menuHelpElement > 0) {
                    SprDrawRect((SprRect *)&menuElementIcons[menuHelpElement], 2 - m->frame.ox, i * 0x18 - m->frame.oy + 2);
                }
                break;
            }
        }
    } else {
        items = m->lines;
        for (i = 0; i < 4; i++) {
            switch (items[i]) {
            case 5:
                for (c = 0; c < 2; c++) {
                    if (menuHelpWeaponIcons[c] == 1) {
                        SprDrawRect((SprRect *)&menuElementIcons[menuHelpElement], c * 0xA0 - m->frame.ox + 0x2A, i * 0x18 - m->frame.oy + 2);
                    }
                }
                break;
            case 6:
                j = 0;
                if (menuHelpElemAffinity[0] > 0) {
                    fx = 0x5C - m->frame.ox;
                    while (j < 8 && menuHelpElemAffinity[j] > 0) {
                        SprDrawRect((SprRect *)&menuElementIcons[menuHelpElemAffinity[j]], fx, i * 0x18 - m->frame.oy + 2);
                        fx += 30.0f;
                        j++;
                    }
                    fx = 0x110 - m->frame.ox;
                    q = j;
                    while (q < 8 && menuHelpElemAffinity[q] < 0) {
                        q++;
                    }
                    if (q - j >= 5) {
                        fx -= (q - j - 4) * 30;
                    }
                } else {
                    fx = 0x5C - m->frame.ox;
                }
                while (j < 8 && menuHelpElemAffinity[j] < 0) {
                    SprDrawRect((SprRect *)&menuElementIcons[-menuHelpElemAffinity[j]], fx, i * 0x18 - m->frame.oy + 2);
                    j++;
                    fx += 30.0f;
                }
                break;
            }
        }
    }
    SprFlush();
    GphTexBind(0xEA);
    if (m->style == 1) {
        for (i = 0; i < 4; i++) {
            switch (items[i]) {
            case 8:
                if (m->kind == 4) {
                    SprDrawRect(&menuHelpVsTypeIcons[menuHelpVsType], 0x76 - m->frame.ox, i * 0x18 - m->frame.oy + 2);
                }
                break;
            }
        }
    } else {
        for (i = 0; i < 4; i++) {
            switch (items[i]) {
            case 5:
                for (c = 0; c < 2; c++) {
                    if (menuHelpWeaponIcons[c] == 2) {
                        SprDrawRect(&menuHelpVsTypeIcons[menuHelpVsType], c * 0xA0 - m->frame.ox + 0x9E, i * 0x18 - m->frame.oy + 2);
                    }
                }
                break;
            case 8:
                if (m->kind == 4) {
                    SprDrawRect(&menuHelpVsTypeIcons[menuHelpVsType], 0x9E - m->frame.ox, i * 0x18 - m->frame.oy + 2);
                } else {
                    SprDrawRect(&menuHelpVsTypeIcons[menuHelpVsType], 0x11C - m->frame.ox, i * 0x18 - m->frame.oy + 2);
                }
                break;
            }
        }
    }
    SprFlush();
    TxtGlyphBufUpload(menuHelpGlyphBuf);
    if (m->style == 1) {
        for (i = 0; i < 4; i++) {
            switch (items[i]) {
            case 4:
                MenuHelpDrawStatNames();
                break;
            case 5:
            case 6:
            case 7:
            case 8:
            case 9:
                if (m->kind == 4 && items[i] == 6) {
                    MenuHelpSetElementColor(((PtyItem *)m->data)->equip->unkF);
                } else {
                    MenuHelpSetColorCyan();
                }
                if (m->kind == 4 && items[i] == 8) {
                    MenuTextDraw(&m->text[16]);
                } else {
                    MenuTextDraw(&m->text[n]);
                    n++;
                }
                sprState.col[0].b = 0x80;
                sprState.col[0].g = 0x80;
                sprState.col[0].r = 0x80;
                MenuTextDraw(&m->text[n]);
                n++;
                break;
            case 2:
                MenuTextDraw(&m->text[n]);
                n++;
                MenuTextDraw(&m->text[n]);
                n++;
                break;
            }
        }
        SprFlush();
        GphTexBind(0xEB);
        for (i = 0; i < 4; i++) {
            switch (items[i]) {
            case 4:
                MenuHelpDrawStatValues();
                break;
            }
        }
        SprFlush();
        GphTexBind(0xEC);
        for (i = 0; i < 4; i++) {
            switch (items[i]) {
            case 7:
                if (m->kind == 4) {
                    SprDrawRect(&menuStatusIcons[menuHelpStatusNo], 0x4A - m->frame.ox, i * 0x18 - m->frame.oy + 2);
                }
                break;
            }
        }
    } else {
        for (i = 0; i < 4; i++) {
            switch (items[i]) {
            case 4:
                MenuHelpDrawStatNames();
                break;
            case 5: {
                s32 c2;
                for (c2 = 0; c2 < 2; c2++) {
                    if (menuHelpWeaponIcons[c2] != 0) {
                        if (menuHelpWeaponIcons[c2] == 1) {
                            MenuHelpSetElementColor(((PtyItem *)m->data)->equip->unkF);
                        } else {
                            MenuHelpSetColorCyan();
                        }
                        if (menuHelpWeaponIcons[c2] == 2) {
                            MenuTextDraw(&m->text[16]);
                        } else {
                            MenuTextDraw(&m->text[n]);
                            n++;
                        }
                        sprState.col[0].b = 0x80;
                        sprState.col[0].g = 0x80;
                        sprState.col[0].r = 0x80;
                        MenuTextDraw(&m->text[n]);
                        n++;
                    }
                }
            }
                break;
            case 1:
                MenuTextDraw(&m->text[n]);
                n++;
                break;
            case 7:
                {
                    s32 o = n * 0x24;
                    View *v = (View *)(base + o);
                    f32 sc = 1.0f;
                    if (!(v->w <= 80.0f)) {
                        sc = 80.0f / v->w;
                    }
                    SprPushTransform(0, v->x * (1.0f - sc), 0.0f, sc, 1.0f);
                    MenuTextDraw(&base[o + 0x34]);
                }
                n++;
                SprPopTransform();
                break;
            case 2:
                MenuTextDraw(&m->text[n]);
                n++;
                MenuTextDraw(&m->text[n]);
                n++;
                MenuTextDraw(&m->text[n]);
                n++;
                MenuTextDraw(&m->text[n]);
                n++;
                break;
            case 8:
                MenuHelpSetColorCyan();
                MenuTextDraw(&m->text[16]);
                sprState.col[0].b = 0x80;
                sprState.col[0].g = 0x80;
                sprState.col[0].r = 0x80;
                MenuTextDraw(&m->text[n]);
                n++;
                break;
            case 9:
                MenuTextDraw(&m->text[n]);
                n++;
                MenuTextDraw(&m->text[n]);
                n++;
                break;
            case 6:
                k = 0;
                if (menuHelpElemAffinity[0] > 0) {
                    {
                        s32 o = n * 0x24;
                        View *v = (View *)(base + o);
                        f32 sc;
                        if (v->w <= 40.0f) {
                            sc = 1.0f;
                        } else {
                            sc = 40.0f / v->w;
                        }
                        SprPushTransform(0, v->x * (1.0f - sc), 0.0f, sc, 1.0f);
                        MenuTextDraw(&base[o + 0x34]);
                    }
                    n++;
                    SprPopTransform();
                    while (k < 8 && menuHelpElemAffinity[k] > 0) {
                        MenuHelpSetElementColor(menuHelpElemAffinity[k]);
                        k++;
                        MenuTextDraw(&m->text[n]);
                        n++;
                    }
                    sprState.col[0].b = 0x80;
                    sprState.col[0].g = 0x80;
                    sprState.col[0].r = 0x80;
                }
                if (k < 8 && menuHelpElemAffinity[k] < 0) {
                    {
                        s32 o = n * 0x24;
                        View *v = (View *)(base + o);
                        f32 sc = 1.0f;
                        if (!(v->w <= 40.0f)) {
                            sc = 40.0f / v->w;
                        }
                        SprPushTransform(0, v->x * (1.0f - sc), 0.0f, sc, 1.0f);
                        MenuTextDraw(&base[o + 0x34]);
                    }
                    n++;
                    SprPopTransform();
                    while (k < 8 && menuHelpElemAffinity[k] < 0) {
                        MenuHelpSetElementColor(-menuHelpElemAffinity[k++]);
                        MenuTextDraw(&m->text[n]);
                        n++;
                    }
                    sprState.col[0].b = 0x80;
                    sprState.col[0].g = 0x80;
                    sprState.col[0].r = 0x80;
                }
                break;
            case 3:
                MenuTextDraw(&m->text[n]);
                n++;
                if (menuHelpTargetIsFoe == 0) {
                    MenuTextSetGradColor(0, 0x40, 0x80);
                    MenuTextSetAddColor(0, 0, 0x20);
                } else {
                    MenuTextSetGradColor(0x80, 0x10, 0x20);
                    MenuTextSetAddColor(0x20, 0, 0);
                }
                MenuTextDraw(&m->text[n]);
                n++;
                MenuTextDraw(&m->text[n]);
                n++;
                MenuTextDraw(&m->text[n]);
                n++;
                break;
            }
        }
        SprFlush();
        GphTexBind(0xEB);
        for (i = 0; i < 4; i++) {
            switch (items[i]) {
            case 4:
                MenuHelpDrawStatValues();
                break;
            case 9:
                MenuNumberDraw(&menuHelpLinkNumField, menuHelpLinkValue);
                SprDrawRect(&menuHelpLinkIcon, menuHelpLinkNumField.x, menuHelpLinkNumField.y);
                break;
            }
        }
        SprFlush();
        GphTexBind(0xEC);
        for (i = 0; i < 4; i++) {
            switch (items[i]) {
            case 5:
                for (c = 0; c < 2; c++) {
                    if (menuHelpWeaponIcons[c] == 3) {
                        SprDrawRect(&menuStatusIcons[menuHelpStatusNo], c * 0xA0 - m->frame.ox + 0x72, i * 0x18 - m->frame.oy + 2);
                    }
                }
                break;
            case 7: {
                s32 flags;
                s32 hi;
                s16 px;
                s16 py;
                s8 b;

                py = i * 0x18 - m->frame.oy + 4;
                flags = ((PtyItem *)m->data)->equip->unk10;
                px = 132.0f - m->frame.ox;
                hi = flags & 0x8000;
                for (b = 0; b < 9; b++) {
                    if (!(flags & (0x10 << b))) {
                        sprState.col[0].b = 0x10;
                        sprState.col[0].g = 0x10;
                        sprState.col[0].r = 0x10;
                    } else {
                        sprState.col[0].b = 0x80;
                        sprState.col[0].g = 0x80;
                        sprState.col[0].r = 0x80;
                    }
                    SprDrawRect(&menuStatusIcons[b], px - 2, py - 2);
                    px += 26;
                }
                if (!hi) {
                    sprState.col[0].b = 0x10;
                    sprState.col[0].g = 0x10;
                    sprState.col[0].r = 0x10;
                } else {
                    sprState.col[0].b = 0x80;
                    sprState.col[0].g = 0x80;
                    sprState.col[0].r = 0x80;
                }
                SprDrawRect(&menuHelpImmunityAllIcon, px - 2, py - 2);
                sprState.col[0].b = 0x80;
                sprState.col[0].g = 0x80;
                sprState.col[0].r = 0x80;
            } break;
            }
        }
    }
    SprFlush();
}
