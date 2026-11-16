/* Port-only C for MiniConcertStageSetupInit (0x0064A498), which the decompilation still has in assembly. */
#include "common.h"
#include "protos/field.h"
#include "field/mini.h"
#include "field/n24.h"

void MiniConcertStageSetupInit(void) {
    MiniConcertWork *w = miniConcertWork;
    MiniConcertStage *s = &w->stage;
    MiniSprite *o;
    u32 i;
    u32 j;

    MiniPanelInit(&s->panels[0], miniConcertData->unk34, miniConcertData->unk38, s->sprites);
    MiniPanelInit(&s->panels[1], miniConcertData->unk34 + 0x20, miniConcertData->unk38, s->sprites);
    MiniPanelInit(&s->panels[2], miniConcertData->unk34 + 0x40, miniConcertData->unk38, s->sprites);
    MiniPanelInit(&s->panels[3], miniConcertData->unk34 + 0x60, miniConcertData->unk38, s->sprites);

    /* The 5x5 roster cells */
    for (i = 0; i < 5; i++) {
        for (j = 0; j < 5; j++) {
            s->sprites[30 + i * 5 + j].drawFlags = MINI_DRAW_ANIM;
        }
    }
    for (i = 0; i < 9; i++) {
        s->unk18C4[i] = -1;
    }

    s->sprites[55].drawFlags |= MINI_DRAW_SPARKLE;

    o = &s->sprites[25];
    MiniSpriteSetCell(o, 1, 1);
    o->stretchSpeed = 0x19;
    o->stretchRect = 0x42;
    o->stretchMode = 0;

    o = &s->sprites[27];
    MiniSpriteSetCell(o, 1, 1);
    o->stretchRect = 0x41;
    o->stretchMode = 0;
    o->stretchSpeed = -0x19;

    s->sprites[26].blinkPeriod = 0xF0;

    o = &s->sprites[5];
    o->stretchRect = 0x35;
    o->stretchMode = 1;
    o->stretchSpeed = 0x19;
    o = &s->sprites[6];
    o->stretchSpeed = 0x2EE0;
    o->stretchMode = 2;
    o->stretchRect = 0x2E;
    o = &s->sprites[7];
    o->stretchRect = 0x37;
    o->stretchMode = 1;
    o->stretchSpeed = 0x19;
    o = &s->sprites[8];
    o->stretchRect = 0x38;
    o->stretchMode = 1;
    o->stretchSpeed = 0x19;

    o = &s->sprites[0];
    MiniSpriteSetMsg(o, (s8)miniConcertLayout->groups[9].first, 6);
    o->active = 1;
    o->textOfsX = 8;
    o->textOfsY = 0xC;
    MiniSpriteSetTextSize(o, 0x18, 0x18);

    /* Carros the party has (global flags 0x538..0x550): their cell number, or -1 */
    for (i = 0; i < 5; i++) {
        for (j = 0; j < 5; j++) {
            s->grid[i][j] = EvtGlobalFlagGet(0x538 + i * 5 + j);
            if (s->grid[i][j] == 1) {
                s->grid[i][j] = i * 5 + j;
                s->unk18F4[i]++;
                s->unk18F9++;
            } else {
                s->grid[i][j] = -1;
            }
        }
    }

    for (i = 0; i < 5; i++) {
        miniConcertWork->actors[i].unk1D = -1;
        o = &s->sprites[15 + i];
        o->sx = 1.25f;
        o->sy = 1.25f;
        o->ofsX = (s32)(o->ofsX + -4.0f);
        o->ofsY = (s32)(o->ofsY + -4.0f);
        o->drawFlags |= MINI_DRAW_ANIM;
    }
    s->unk18FB = -1;
    s->unk18FA = -1;
    MiniConcertRosterRefresh();
    s->unk18C0 = &s->sprites[57];
    MiniMsgBoxSet(&s->sprites[57], 8);
    MiniSpriteSetMsgGroup(&s->sprites[62], 0, 7);
}
