#include "common.h"
#include "protos/field.h"
#include "field/n24.h"


void MiniConcertFormationCheck(void) {
    MiniConcertStage *s = &miniConcertWork->stage;
    MiniSprite *o;
    u32 i;
    s32 cnt;
    s32 k;

    for (i = 0; i < 5; i++) {
        if (s->unk18EE[i] == 5) {
            break;
        }
    }
    if (i == 5) {
        for (i = 0; i < 5; i++) {
            s->sprites[20 + i].active = 0;
        }
        return;
    }
    s->unk18FA = i;
    cnt = 0;
    k = 0;
    for (i = 0; i < 9; i++) {
        if (miniConcertData->unk24[s->unk18FA][i] == 0) {
            o = &s->sprites[20 + k];
            o->active = 1;
            o->rect = 0x3E;
            MiniSpriteSetCell(o, i % 3, i / 3);
            k++;
        } else if (s->unk18C4[i] != -1) {
            cnt++;
        }
    }
    s->unk18FB = (cnt == 5) ? s->unk18FA : -1;
}
