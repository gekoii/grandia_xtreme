#include "common.h"
#include "protos/field.h"
#include "save.h"
#include "field/n25.h"


void MiniBallJudge(void) {
    MiniBallWork *d = miniBallWork;
    MiniBallGame *s;
    u32 i;
    MiniBallPrize *sl;

    if (d->frame == miniBallData->set->judgeTime) {
        s = &d->game;
        if (s->winColor == s->color) {
            MiniMsgBoxSet(s->msgBox, 7);
            SndSePlay3D(0x814, 0, 0, NULL);
        } else {
            MiniMsgBoxSet(s->msgBox, 8);
            SndSePlay3D(0x815, 0, 0, NULL);
            sysSaveData.miniBallResult = 1;
            for (i = 0; i < 6; i++) {
                sl = &d->game.prizes[i];
                if (sl->times[2] >= miniBallWork->frame) {
                    sl->spinning = 1;
                    sl->times[2] = miniBallWork->frame;
                }
            }
        }
        s->judged = 1;
    }
}
