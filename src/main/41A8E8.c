#include "common.h"
#include "protos/main.h"
#include "party.h"
#include "main/n02.h"

extern char menuHelpElementNames[];
extern char menuHelpElementLabels[];
extern char menuHelpStrElementSuffix[];

void MenuHelpAddElement(s8 *arg0, s16 arg1, s16 arg2) {
    MenuHelp *b = &menuHelp;
    s8 i = *arg0;
    PtyEquipData *p = ((PtyItem *)b->data)->equip;
    s8 k;

    b->text[i].style = 1;
    b->text[i].x = arg1 - b->frame.ox;
    b->text[i].y = arg2 - b->frame.oy;
    k = p->unkF;
    if (k < 5) {
        TxtObjSetString(&b->text[i], menuHelpGlyphBuf, menuHelpElementLabels + k);
    } else {
        TxtObjSetString(&b->text[i], menuHelpGlyphBuf, menuHelpElementNames + k * 9);
    }
    i++;
    b->text[i].style = 1;
    b->text[i].x = b->text[i - 1].x + b->text[i - 1].width;
    b->text[i].y = b->text[i - 1].y;
    TxtObjSetString(&b->text[i], menuHelpGlyphBuf, menuHelpStrElementSuffix);
    *arg0 = i + 1;
}
