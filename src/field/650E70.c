#include "common.h"
#include "protos/field.h"
#include "field/n25.h"


s32 MiniBallChooseColor(void) {
    MiniSprite *o2 = &miniBallWork->game.sprites[28];
    MiniSprite *o1 = &miniBallWork->game.sprites[29];
    MiniBallGame *s = &miniBallWork->game;
    MiniInput *pad = &miniBallUi->input;
    u8 mode = s->chooseMode;
    u16 btn;

    switch (mode) {
    case 1: {
            btn = pad->trigger;
            if (btn & 0x20) {
                s->msgBox->active = 0;
                s->color = o1->cellX;
                o1->fxFlags |= 8;
                MiniBallReelSetOrder();
                s->sprites[59].active = 0;
                s->sprites[60].active = 0;
                s->sprites[61].active = 0;
                s->sprites[62].active = 0;
                SndSePlay3D(0x80E, 0, 0, NULL);
                return 1;
            }
            if (btn & 0x40) {
                MiniMsgBoxSet(s->msgBox, 1);
                o1->fxFlags |= 8;
                o2->blinkPeriod = 90;
                s->chooseMode = 0;
                s->sprites[59].active = 0;
                s->sprites[60].active = 0;
                s->sprites[61].active = 0;
                s->sprites[62].active = 0;
                SndSePlay3D(0x80F, 0, 0, NULL);
            }
        }
        break;
    case 0:
        if (MiniCursorMove(o1, 0, 0, 5, 0, 5, 0) != 0) {
            MiniSpriteSetCell(o2, o1->cellX, 0);
            o2->rect = o1->cellX + 0x27;
            SndSePlay3D(0x80D, 0, 0, NULL);
        } else if (pad->trigger & 0x20) {
            MiniMsgBoxSet(s->msgBox, 2);
            o1->fxFlags &= ~8;
            o2->blinkPeriod = 30;
            s->chooseMode = 1;
            s->sprites[59].active = 1;
            s->sprites[60].active = 1;
            s->sprites[61].active = 1;
            s->sprites[62].active = 1;
            SndSePlay3D(0x80E, 0, 0, NULL);
        }
        break;
    }
    return 0;
}
