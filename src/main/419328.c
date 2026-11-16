#include "common.h"
#include "protos/main.h"
#include "main/n02.h"

extern u8 menuTacticsNames[];
extern u8 menuTacticsTitles[];
extern u8 menuTacticsDescs[];

void MenuHelpBuildTactics(void) {
    MenuHelp *w;
    s32 i;
    s8 f;

    w = &menuHelp;
    f = 1;
    i = 0;
    w->text[i].x = 0;
    w->text[i].y = 6 - w->frame.oy;
    w->text[i].style = 2;
    TxtObjSetString(&w->text[i], menuHelpGlyphBuf, menuTacticsNames + w->kind * 0x18);
    i = 1;
    w->text[i].style = f;
    w->text[i].x = 0x6A - w->frame.ox;
    w->text[i].y = 6 - w->frame.oy;
    TxtObjSetString(&w->text[i], menuHelpGlyphBuf, menuTacticsTitles + w->kind * 0x2D);
    i = 2;
    w->text[i].style = f;
    w->text[i].x = 0;
    w->text[i].y = 0x1E - w->frame.oy;
    /* through a cast: a direct call to the void function becomes a tail call */
    ((s32 (*)(void *, void *, void *))TxtObjSetString)(&w->text[i], menuHelpGlyphBuf, menuTacticsDescs + w->kind * 0x43);
}
