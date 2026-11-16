#include "common.h"
#include "protos/main.h"
#include "party.h"
#include "main/n02.h"


void MenuHelpAddDesc(s8 *arg0, s16 arg1, s16 arg2) {
    u8 *s;
    s8 idx;
    s32 j;
    MenuHelp *b;

    b = &menuHelp;
    s = NULL;
    idx = *arg0;
    switch (b->kind) {
    case 0:
    case 1:
        s = (u8 *)((PtyMove *)b->data)->helpDesc;
        break;
    case 2:
        s = (u8 *)((PtySkillDef *)b->data)->helpDesc;
        break;
    case 3:
    case 4:
    case 5:
        s = (u8 *)((PtyItem *)b->data)->helpDesc;
        break;
    }
    if (s == NULL) {
        return;
    }
    b->text[idx].style = 1;
    b->text[idx].x = arg1 - b->frame.ox;
    b->text[idx].y = arg2 - b->frame.oy;
    TxtObjSetString(&b->text[idx], menuHelpGlyphBuf, s);
    idx++;
    while (*s != 0) {
        s++;
    }
    s++;
    j = idx - 1;
    if (b->style == 0) {
        b->text[idx].style = 1;
        b->text[idx].x = b->text[j].x + b->text[j].width;
        b->text[idx].y = b->text[j].y;
        TxtObjSetString(&b->text[idx], menuHelpGlyphBuf, s);
        idx++;
        while (*s != 0) {
            s++;
        }
        s++;
        j = idx - 1;
        b->text[idx].style = 1;
        b->text[idx].x = arg1 - b->frame.ox;
        b->text[idx].y = b->text[j].y + 0x18;
        TxtObjSetString(&b->text[idx], menuHelpGlyphBuf, s);
        idx++;
        while (*s != 0) {
            s++;
        }
        s++;
        j = idx - 1;
    }
    b->text[idx].style = 1;
    b->text[idx].x = arg1 - b->frame.ox;
    b->text[idx].y = b->text[j].y + 0x18;
    TxtObjSetString(&b->text[idx], menuHelpGlyphBuf, s);
    idx++;
    *arg0 = idx;
}
