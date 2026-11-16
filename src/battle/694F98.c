#include "common.h"
#include "protos/battle.h"
#include "battle/n29.h"
#include "sys.h"


void BtlUnitInfoInit(void) {
    s32 i;
    s32 j;
    s32 hue;
    u32 c;
    s32 six;
    SprColorQuad *g;
    TxtObj *b;

    btlUnitInfo.unit = NULL;
    TxtGlyphBufInit(&btlUnitInfoGlyphBuf, sysFontImage2, 0, 0x33);
    six = 6;
    hue = 0xD000;
    TxtGlyphBufInit(&btlUnitInfoSkillGlyphBuf, &sysFontImage2[1], 0x80, 0xEB);
    g = btlUnitInfoGaugeQuads;
    for (i = 0; i < 4; i++) {
        g->w = six;
        c = UtilHsvToRgb(hue, 0.1f, 0.9f);
        g->col[2].w = c;
        g->col[0].w = c;
        c = UtilHsvToRgb(hue, 1.0f, 0.8f);
        hue += 0x4000;
        g->col[3].w = c;
        g->col[1].w = c;
        g++;
    }
    b = btlUnitInfoSkillTexts;
    for (j = 0; j < 6; j++) {
        b->alignX = 0;
        b->alignY = 0;
        b->x = 0x18;
        b->y = (f32)(j * 18) + 18.0f + 2.0f;
        b->scale = 0.75f;
        b->style = 1;
        b->spacing = 2;
        b++;
    }
}
