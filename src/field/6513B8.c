#include "common.h"
#include "protos/field.h"
#include "field/n25.h"


void MiniBallCheckWin(void) {
    MiniBallGame *s = &miniBallWork->game;
    u32 i;
    u8 id;
    MiniBallWork *p;

    if (s->reelsStopped != 0) {
        if (s->winColor == s->color) {
            id = s->winColor;
            i = 0;
            if (s->prizes[0].reel != id) {
                do {
                    i++;
                } while (i < 6 && s->prizes[i].reel != id);
            }
            /* &s->sprites[43 + i]; indexing the array does not match */
            {
                u32 off = i * 0x60 + 0x1200;
                ((MiniSprite *)((u8 *)s + off))->fxFlags |= 2;
            }
            MiniMsgBoxSet(s->msgBox, 9);
            SndSePlay3D(0x80B, 0, 0, NULL);
        }
        p = miniBallWork;
        p->subState = 4;
        p->frame = 0;
    }
}
