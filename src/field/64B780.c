#include "common.h"
#include "protos/field.h"
#include "field/n24.h"


void MiniConcertCarroRemove(void) {
    MiniInput *pad = &miniConcertUi->input;
    MiniConcertWork *m = miniConcertWork;
    MiniConcertStage *s = &m->stage;
    MiniSprite *cur;
    MiniSprite *o;
    MiniConcertActor *e;
    u32 j;

    if (pad->trigger & 0x40) {
        MiniConcertStageMenuBack();
        return;
    }
    cur = &s->sprites[27];
    if (MiniCursorMove(cur, 0, 0, 2, 2, 5, 1)) {
        MiniMsgBoxSet(s->unk18C0, 0x17);
        SndSePlay3D(0x818, 0, 0, 0);
        return;
    }
    if (pad->trigger & 0x20) {
        s->sel.b.idx = cur->cellX + cur->cellY * 3;
        s->unk18ED = s->unk18C4[s->sel.b.idx];
        if (s->unk18ED == -1) {
            SndSePlay3D(0x7D4, 0, 0, 0);
            return;
        }
        s->unk18C4[s->sel.b.idx] = -1;
        s->unk18F3--;
        s->grid[s->unk18ED / 5][s->unk18ED % 5] = s->unk18ED;
        for (j = 0; j < 5; j++) {
            o = &s->sprites[15 + j];
            if (o->active && MINI_SPRITE_CELL(o) == MINI_SPRITE_CELL(cur)) {
                o->active = 0;
                e = &miniConcertWork->actors[j];
                e->unk1C = -1;
                e->unk1D = -1;
                s->unk18EE[e->unk1E]--;
                MiniConcertRosterRefresh();
                MiniConcertFormationCheck();
                MiniMsgBoxSet(s->unk18C0, 0x18);
                MiniConcertCarroSePlay(e->unk1E, 2);
                if (s->unk18F3 == 0) {
                    s->unk18E6 = 0;
                    s->unk18E7 = 0;
                    s->sprites[27].active = 0;
                    s->sprites[13].fxFlags |= 2;
                    MiniMsgBoxSet(s->unk18C0, 0xC);
                }
                return;
            }
        }
    }
}
