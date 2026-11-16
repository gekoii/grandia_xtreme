#include "common.h"
#include "protos/main.h"
#include "party.h"
#include "main/n02.h"

typedef struct {
    u8 s[7];
} Name;

extern Name menuHelpVsTypeNames[];
extern u8 menuHelpStrReducesDamage[];
extern s8 menuHelpVsType;

void MenuHelpAddDamageReduce(s8 *arg0, s16 x, s16 y) {
    s8 n;
    s8 k;
    MenuHelp *w;

    w = &menuHelp;
    n = *arg0;
    w->text[16].x = x - w->frame.ox;
    w->text[16].y = y - w->frame.oy;
    menuHelpVsType = k = ((PtyItem *)w->data)->equip->effectType + 0xD9;
    TxtObjSetString(&w->text[16], menuHelpGlyphBuf, &menuHelpVsTypeNames[k]);
    w->text[n].style = 1;
    w->text[n].x = w->text[16].x + w->text[16].width;
    w->text[n].y = w->text[16].y;
    TxtObjSetString(&w->text[n], menuHelpGlyphBuf, menuHelpStrReducesDamage);
    *arg0 = n + 1;
}
