#include "common.h"
#include "protos/field.h"
#include "field/n25.h"

void MiniBallReelsUpdate(void) {
    MiniBallGame *s = &miniBallWork->game;
    MiniBallPrize *sl;
    u32 i;
    u32 n;
    u32 step;
    u32 cnt;
    s32 flag;
    u8 v;

    flag = 0;
    for (i = 0; i < 6; i++) {
        sl = &s->prizes[i];
        if (sl->spinning == 0) {
            if (miniBallWork->frame != sl->times[0]) {
                continue;
            }
            n = miniBallWork->frame + 1;
            step = 3;
            cnt = 0;
            while (n < sl->times[2]) {
                n += step;
                cnt++;
                if (sl->times[1] < n) {
                    step += 5;
                }
            }
            cnt = cnt % 6;
            sl->spinning = 1;
            sl->nextStep = miniBallWork->frame + 1;
            sl->stepLen = 3;
            sl->pos = (sl->reel - cnt + 10) % 6;
        } else {
            if (sl->times[2] < miniBallWork->frame) {
                continue;
            }
            if (miniBallWork->frame == sl->times[2]) {
                v = sl->reel;
                if (++s->reelsDone == 6) {
                    s->reelsStopped = 1;
                }
            } else {
                if (miniBallWork->frame != sl->nextStep) {
                    continue;
                }
                flag = 1;
                if (++sl->pos == 6) {
                    sl->pos = 0;
                }
                sl->nextStep += sl->stepLen;
                v = sl->pos;
                if (sl->times[1] < miniBallWork->frame) {
                    sl->stepLen += 5;
                }
            }
            {
                MiniSprite *m = &s->sprites[37 + i];
                m->rect = v + 11;
                m->active = 1;
            }
        }
    }
    if (flag == 1) {
        SndSePlay3D(0x812, 0, 0, NULL);
    }
}
