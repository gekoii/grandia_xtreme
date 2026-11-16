#include "common.h"
#include "protos/main.h"
#include "main/n02.h"

/* Head shared by PtyMove and PtyItemUse: the item kind (3) shows its use
 * record (PtyItem.use, the +0x1C of this view). */
typedef struct Info {
    u8 pad0;
    s8 b1;
    s8 b2;
    u8 pad3[0x1C - 3];
    struct Info *alt;
} Info;

extern char menuHelpTargetNames[];
extern char menuHelpRangeNames[];
extern s8 menuHelpTargetIsFoe;

s32 MenuHelpAddTargetRange(s8 *p, s16 x, s16 y) {
    s8 i;
    s8 a;
    s8 b;
    Info *q;
    MenuHelp *g;

    g = &menuHelp;
    i = *p;
    if (g->kind == 3) {
        q = ((Info *)g->data)->alt;
        if (q == NULL) {
            return 0;
        }
        a = q->b2;
    } else {
        q = g->data;
        a = q->b2;
    }
    b = q->b1;
    switch (b) {
    case 1:
    case 3:
    case 5:
        menuHelpTargetIsFoe = 0;
        break;
    case 2:
    case 4:
        menuHelpTargetIsFoe = 1;
        break;
    }
    g->text[i].style = 1;
    g->text[i].x = x - g->frame.ox;
    g->text[i].y = y - g->frame.oy;
    TxtObjSetString(&g->text[i], menuHelpGlyphBuf, menuHelpTargetNames);
    i++;
    g->text[i].style = 0x12;
    g->text[i].x = g->text[i - 1].x + 0x3C;
    g->text[i].y = g->text[i - 1].y;
    TxtObjSetString(&g->text[i], menuHelpGlyphBuf, menuHelpTargetNames + b * 9);
    i++;
    g->text[i].style = 1;
    g->text[i].x = x - g->frame.ox + 0x8C;
    g->text[i].y = y - g->frame.oy;
    TxtObjSetString(&g->text[i], menuHelpGlyphBuf, menuHelpRangeNames);
    i++;
    g->text[i].style = 2;
    g->text[i].x = g->text[i - 1].x + 0x50;
    g->text[i].y = g->text[i - 1].y;
    TxtObjSetString(&g->text[i], menuHelpGlyphBuf, menuHelpRangeNames + a * 13);
    *p = i + 1;
    return 1;
}
