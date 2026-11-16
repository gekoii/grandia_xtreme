#include "common.h"
#include "protos/field.h"
#include "field/n24.h"


s32 MiniConcertStageMenuInput(void) {
    MiniConcertWork *m = miniConcertWork;
    MiniSprite *cur = &m->stage.sprites[13];
    MiniConcertStage *s = &m->stage;
    MiniInput *pad = &miniConcertUi->input;
    u32 i;

    switch (s->unk18E6) {
    case 1:
        switch (cur->cellX) {
        case 0:
            MiniConcertCarroChange();
            break;
        case 1:
            MiniConcertFormationChange();
            break;
        case 2:
            MiniConcertCarroRemove();
            break;
        case 3:
            if (pad->trigger & 0x20) {
                return 1;
            }
            if (pad->trigger & 0x40) {
                s->unk18E6 = 0;
                cur->fxFlags |= 2;
                MiniMsgBoxSet(s->unk18C0, 0xD);
                s->sprites[60].active = 0;
                s->sprites[61].active = 0;
                s->sprites[62].active = 0;
                s->sprites[63].active = 0;
                SndSePlay3D(0x81A, 0, 0, 0);
                return 0;
            }
            break;
        }
        return 0;
    case 0:
        if (MiniCursorMove(cur, 0, 0, 3, 0, 5, 0)) {
            SndSePlay3D(0x817, 0, 0, 0);
            MiniSpriteSetMsg(&s->sprites[0], cur->cellX + miniConcertLayout->groups[9].first, 6);
            MiniMsgBoxSet(s->unk18C0, cur->cellX + 10);
            for (i = 5; i < 13; i++) {
                s->sprites[i].active = 0;
            }
            s->sprites[cur->cellX + 5].active = 1;
            s->sprites[cur->cellX + 9].active = 1;
            break;
        }
        if (pad->trigger & 0x20) {
            switch (cur->cellX) {
            case 0:
                MiniMsgBoxSet(s->unk18C0, 0xE);
                s->sprites[55].active = 1;
                s->sprites[29].active = 1;
                break;
            case 1:
                if (s->unk18F3 == 0) {
                    SndSePlay3D(0x7D4, 0, 0, 0);
                    return 0;
                }
                MiniMsgBoxSet(s->unk18C0, 0x14);
                s->sprites[27].active = 1;
                break;
            case 2:
                if (s->unk18F3 == 0) {
                    SndSePlay3D(0x7D4, 0, 0, 0);
                    return 0;
                }
                MiniMsgBoxSet(s->unk18C0, 0x17);
                s->sprites[27].active = 1;
                break;
            case 3:
                if (s->unk18F3 == 0) {
                    MiniMsgBoxSet(s->unk18C0, 0x1A);
                    SndSePlay3D(0x7D4, 0, 0, 0);
                    return 0;
                }
                MiniMsgBoxSet(s->unk18C0, 0x19);
                s->sprites[60].active = 1;
                s->sprites[61].active = 1;
                s->sprites[62].active = 1;
                s->sprites[63].active = 1;
                break;
            }
            SndSePlay3D(0x819, 0, 0, 0);
            s->unk18E6 = 1;
            cur->fxFlags &= ~2;
        }
        break;
    }
    return 0;
}
