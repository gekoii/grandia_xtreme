#include "common.h"
#include "protos/field.h"
#include "field/menu.h"



static inline void copyq(void *dst, void *src) {
    __asm__ volatile("lq $2, 0(%1)\n\tsq $2, 0(%0)" : : "r"(dst), "r"(src) : "$2", "memory");
}

void FldMenuWinClose(u8 arg0, u8 arg1, s32 arg2) {
    FldMenuWin *e = &fldMenuWins[arg0];

    if (e->state != 0) {
    if (e->state < 5) {
        if (((u8 *)e)[3] == 9 /* open type: pad3 in FldMenuWin */ || (e->flags & 0x2000)) {
            e->state = 0;
            e->active = 0;
            return;
        }
        e->timer = arg1;
        e->state = 5;
        e->unk34 = 0;
        copyq(e->openStart, e->pos);
        copyq(e->openEnd, e->openStart);
        if (arg2 == 8) {
            arg2 = VuRandInt(8);
        }
        switch (arg2) {
        case 0:
            e->openEnd[0] -= 128.0f;
            break;
        case 1:
            e->openEnd[0] += 128.0f;
            break;
        case 2:
            e->openEnd[1] -= 128.0f;
            break;
        case 3:
            e->openEnd[1] += 128.0f;
            break;
        }
    }
    }
}
