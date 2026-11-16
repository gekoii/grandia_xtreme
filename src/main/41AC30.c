#include "common.h"
#include "protos/main.h"
#include "party.h"
#include "main/n02.h"

extern char menuHelpStatusNames[][17];
extern s8 menuHelpStatusNo;

void MenuHelpAddStatusEffect(s8 *arg0, s16 arg1, s16 arg2) {
    MenuHelp *b = &menuHelp;
    s8 k = 4;
    s8 i = *arg0;
    PtyEquipData *p = ((PtyItem *)b->data)->equip;
    s32 f;

    b->text[i].style = 1;
    f = p->unk10;
    b->text[i].x = arg1 - b->frame.ox;
    b->text[i].y = arg2 - b->frame.oy;
    for (; k < 16; k++) {
        if ((f >> k) & 1) {
            break;
        }
    }
    TxtObjSetString(&b->text[i], menuHelpGlyphBuf, menuHelpStatusNames[k]);
    menuHelpStatusNo = k - 4;
    i++;
    b->text[i].style = 1;
    b->text[i].x = b->text[i - 1].x + b->text[i - 1].width;
    b->text[i].y = b->text[i - 1].y;
    TxtObjSetString(&b->text[i], menuHelpGlyphBuf, menuHelpStatusNames[16]);
    *arg0 = i + 1;
}
