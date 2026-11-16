#include "common.h"
#include "protos/field.h"
#include "field/n25.h"


void MiniBallShootInput(void) {
    u32 t = miniBallWork->frame;
    u32 lim = miniBallData->set->shootEnd;
    MiniBallGame *s;
    MiniInput *pad;
    s32 max;
    u32 n;

    if (lim < t) return;
    s = &miniBallWork->game;
    pad = &miniBallUi->input;
    if (t == lim) {
        MiniMsgBoxSet(s->msgBox, 5);
        s->sprites[29].fxFlags &= ~8;
        return;
    }
    max = 20;
    /* volatile read: launched is loaded here, not CSE'd with the count below (plain read 54 words off) */
    if (*(volatile u8 *)&s->launched == max) {
        if (pad->trigger & 0x20) {
            SndSePlay3D(0x80C, 0, 0, NULL);
        }
        return;
    }
    if (pad->trigger & 0x20) {
        if (s->gold < s->cost) {
            MiniMsgBoxSet(s->msgBox, 6);
            SndSePlay3D(0x80C, 0, 0, NULL);
        } else {
            n = s->launched;
            s->gold -= s->cost;
            MiniBallLaunch(&s->balls[n], &s->sprites[n + 1], s->color);
            s->launched++;
            SndSePlay3D(0x810, 0, 0, NULL);
            if (s->launched == max) {
                MiniMsgBoxSet(s->msgBox, 4);
                s->sprites[29].fxFlags &= ~8;
            }
        }
    }
    if (s->gold < s->cost) {
        s->sprites[29].fxFlags &= ~8;
    }
}
