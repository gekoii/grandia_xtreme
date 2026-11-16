#include "common.h"
#include "protos/field.h"
#include "field/n25.h"


void MiniBallPanelsInit(void) {
    MiniBallWork *d;
    MiniBallGame *s;
    MiniSprite *o49;
    MiniSprite *o50;
    MiniSprite *o51;
    MiniSprite *o53;
    MiniSprite *o61;
    MiniSprite *o56;
    MiniSprite *o;
    MiniBallPrize *sl;
    MiniSprite *objs;
    u32 i;
    s32 k;

    d = miniBallWork;
    o61 = &d->game.sprites[61];
    o49 = &d->game.sprites[49];
    s = &d->game;
    o50 = &s->sprites[50];
    o51 = &s->sprites[51];
    o53 = &s->sprites[53];
    o56 = &s->sprites[56];
    i = 0;
    k = 5;
    objs = s->sprites;
    s->reelsStopped = 0;
    MiniPanelInit(&s->panels[0], &miniBallData->panels[0], miniBallData->cells, objs);
    MiniPanelInit(&s->panels[1], &miniBallData->panels[1], miniBallData->cells, objs);
    MiniPanelInit(&s->panels[2], &miniBallData->panels[2], miniBallData->cells, objs);
    MiniPanelInit(&s->panels[3], &miniBallData->panels[3], miniBallData->cells, objs);
    MiniPanelInit(&s->panels[4], &miniBallData->panels[4], miniBallData->cells, objs);
    MiniPanelInit(&s->panels[5], &miniBallData->panels[5], miniBallData->cells, objs);
    MiniPanelInit(&s->panels[6], &miniBallData->panels[6], miniBallData->cells, objs);
    MiniPanelInit(&s->panels[7], &miniBallData->panels[7], miniBallData->cells, objs);
    MiniPanelInit(&s->panels[9], &miniBallData->panels[9], miniBallData->cells, objs);
    s->sprites[28].blinkPeriod = 90;
    s->sprites[29].drawFlags |= 0x10;
    MiniBallRollPrizes();
    MiniBallShufflePrizes();
    for (; i < 6; i++) {
        o = &s->sprites[43 + i];
        sl = &d->game.prizes[i];
        MiniSpriteSetText(o, (u8 *)sl->name, k++, 1);
        o->textOfsX = 0x20;
        o->textOfsY = 2;
        MiniSpriteSetTextSize(o, 20, 20);
        o->rect = sl->icon;
        o->icon = 0xF0;
    }
    s->msgBox = o56;
    s->panels[7].showIcons = 1;
    MiniMsgBoxSet(o56, 1);
    MiniMsgBoxHide(s->msgBox);
    MiniSpriteSetMsgGroup(o61, 0, k + 14);
    MiniSpriteSetMsgGroup(o49, 13, k++);
    MiniSpriteSetMsgGroup(o50, 14, k++);
    MiniSpriteSetMsgGroup(o51, 15, k++);
    MiniSpriteSetTextSize(o49, 24, 24);
    MiniSpriteSetTextSize(o50, 20, 20);
    MiniSpriteSetTextSize(o51, 20, 20);
    MiniSpriteSetMsgGroup(o53, 11, k + 13);
    MiniSpriteSetTextSize(o53, 20, 20);
    s->chooseMode = 0;
}
