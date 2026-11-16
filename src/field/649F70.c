#include "common.h"
#include "protos/field.h"
#include "field/n24.h"


void MiniConcertSongSelectInit(void) {
    MiniConcertWork *m = miniConcertWork;
    MiniConcertMenu *s = &m->menu;
    MiniSprite *o9 = &s->sprites[9];
    MiniSprite *o6 = &s->sprites[6];
    MiniSprite *o = &s->sprites[7];
    MiniSprite *o12 = &s->sprites[12];
    u8 *list;
    u32 i;

    s->unk5A5 = miniConcertData->rules->unk1D;
    MiniPanelInit(&s->panels[0], miniConcertData->unk2C, miniConcertData->unk30, s->sprites);
    MiniPanelInit(&s->panels[1], miniConcertData->unk2C + 0x20, miniConcertData->unk30, s->sprites);
    list = m->menu.unk5A0;
    o->drawFlags |= 0x10;
    for (i = 0; i < 5; i++) {
        miniConcertWork->unk40[i] = EvtGlobalFlagGet(i + 0x4D0);
        if (miniConcertWork->unk40[i] == 1) {
            list[miniConcertWork->unk45] = i;
            miniConcertWork->unk45++;
        }
    }
    for (i = 0; i < miniConcertWork->unk45; i++) {
        o = &s->sprites[i + 1];
        MiniSpriteSetMsg(o, s->unk5A0[i] + miniConcertLayout->groups[1].first, i + 6);
        o->active = 1;
        o->textOfsX = 0x48;
        o->textOfsY = 6;
        MiniSpriteSetTextSize(o, 0x18, 0x18);
    }
    MiniMsgBoxSet(o9, 3);
    MiniSpriteSetMsg(o6, (s8)miniConcertLayout->groups[2].first, 0xB);
    MiniSpriteSetMsgGroup(o12, 0, 0xC);
}
