#include "common.h"
#include "protos/main.h"
#include "sys.h"
#include "main/n02.h"


void MenuHelpInit(void) {
    MenuHelp *b;
    TxtObj *p;
    MenuStatLine *q;
    s32 i;
    s32 j;

    b = &menuHelp;
    menuHelp.state = 0;
    TxtGlyphBufInit(menuHelpGlyphBuf, menuHelpGlyphImage, 0, 0x7F);
    p = b->text;
    for (j = 0; j < 16; j++) {
        p->alignX = p->alignY = 0.0f;
        p->scale = 0.8333333f;
        p++->spacing = 2;
    }
    b->text[16].alignX = 0.0f;
    b->text[16].alignY = -0.375f;
    b->text[16].scale = 0.6666667f;
    b->text[16].style = 2;
    b->text[16].spacing = 2;
    q = menuHelpStatLines;
    for (i = 0; i < 4; i++) {
        q->text.alignY = b->text[16].alignX;
        q->text.alignX = b->text[16].alignX;
        q->text.scale = 0.8333333f;
        q->text.spacing = 2;
        q->text.style = 0x12;
        q++;
    }
    MenuFrameInit(&b->frame, 0, 0);
    menuHelpFrame = 0;
}
