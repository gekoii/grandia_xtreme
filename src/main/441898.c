#include "common.h"
#include "main/n06.h"
#include "protos/main.h"


static inline void copyq(void *dst, void *src) {
    __asm__ volatile("lq $2, 0(%1)\n\tsq $2, 0(%0)" : : "r"(dst), "r"(src) : "$2", "memory");
}

s32 SndSeSetPos(u8 arg0, s32 arg1, VuVec4 *arg2) {
    arg0 &= 3;
    if (SndSeGetSlotState(arg0, arg1) == 0) {
        return -3;
    }
    if (arg2 == NULL) {
        return -1;
    }
    sndSeSlots[arg0].u.s.slots[arg1].state = 5;
    { SndSeSlot *it = &sndSeSlots[arg0].u.s.slots[arg1]; copyq(it->pos, arg2); }
    return arg1;
}
