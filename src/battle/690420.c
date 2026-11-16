#include "common.h"
#include "protos/battle.h"
#include "battle/n29.h"
#include "sys.h"


void BtlCmdListInit(void) {
    s32 i;
    s32 j;
    s32 k;
    BtlCmdList *b = &btlCmdList;
    BtlCmdListEntry *r;
    TxtObj *s;
    TxtObj *p;
    BtlCmdListEntry *rp;
    TxtObj *t;
    SprColorQuad *pp;

    b->unit = 0;
    r = b->entries;
    s = b->titles;
    b->mode = 0;
    for (i = 0; i < 4; i++) {
        btlCmdListReservedItem[i] = -1;
    }
    TxtGlyphBufInit(btlCmdListGlyphBufs, sysFontImage, 0, 0xC3);
    TxtGlyphBufInit(&btlCmdListGlyphBufs[1], sysFontImage, 0xC4, 0x187);
    p = s;
    for (j = 1; j >= 0; j--) {
        p->alignX = 0.5f;
        p->alignY = 0.0f;
        p->x = 0;
        p->y = 0;
        p->scale = 1.0f;
        p->style = 0x22;
        p->spacing = 2;
        p++;
    }
    rp = r;
    for (k = 0; k < 12; k++) {
        t = &rp->texts[0];
        t->alignX = 0.0f;
        t->alignY = 1.0f;
        t->x = 0;
        t->y = 0;
        t->scale = 0.8333333f;
        t->style = 2;
        t->spacing = 2;
        t = &rp->texts[1];
        t->alignX = 0.0f;
        t->alignY = 1.0f;
        t->x = 0;
        t->y = 0;
        t->scale = 0.8333333f;
        t->style = 2;
        t->spacing = 2;
        t = &rp->texts[2];
        t->alignX = 0.5f;
        t->alignY = 1.0f;
        t->x = 0;
        t->y = 0;
        t->scale = 0.8333333f;
        t->style = 2;
        t->spacing = 2;
        t = &rp->texts[3];
        t->alignX = 0.5f;
        t->alignY = 1.0f;
        t->x = 0;
        t->y = 0;
        t->scale = 0.6666667f;
        t->style = 2;
        t->spacing = 2;
        pp = &rp->quad;
        pp->h = 16;
        pp->originX = -14;
        pp->originY = 12;

        rp++;
    }
}
