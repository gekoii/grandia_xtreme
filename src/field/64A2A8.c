#include "common.h"
#include "protos/field.h"
#include "field/n24.h"


s32 MiniConcertSongSelectInput(void) {
    MiniConcertWork *m = miniConcertWork;
    MiniConcertMenu *s = &m->menu;
    MiniSprite *cur = &s->sprites[7];
    u16 btn;

    switch (s->unk5A6) {
    case 1:
        btn = miniConcertUi->input.trigger;
        if (btn & 0x20) {
            SndSePlay3D(0x819, 0, 0, 0);
            return 1;
        }
        if (btn & 0x40) {
            s->unk5A6 = 0;
            cur->fxFlags |= 2;
            MiniSpriteSetMsg(&s->sprites[9], (s8)miniConcertLayout->groups[7].first, 0);
            s->sprites[10].active = 0;
            s->sprites[11].active = 0;
            s->sprites[12].active = 0;
            s->sprites[13].active = 0;
            SndSePlay3D(0x81A, 0, 0, 0);
        }
        break;
    case 0:
        if (MiniCursorMove(cur, 0, 0, 0, m->unk45 - 1, 5, 0)) {
            MiniSpriteSetMsg(&s->sprites[9], s->unk5A0[cur->cellY] + miniConcertLayout->groups[4].first, 0);
            SndSePlay3D(0x817, 0, 0, 0);
        } else if (miniConcertUi->input.trigger & 0x20) {
            miniConcertWork->songNo = s->unk5A0[cur->cellY];
            s->unk5A6 = 1;
            cur->fxFlags &= ~2;
            MiniSpriteSetMsg(&s->sprites[9], s->unk5A0[cur->cellY] + miniConcertLayout->groups[5].first, 0);
            s->sprites[10].active = 1;
            s->sprites[11].active = 1;
            s->sprites[12].active = 1;
            s->sprites[13].active = 1;
            SndSePlay3D(0x819, 0, 0, 0);
        }
        break;
    }
    return 0;
}
