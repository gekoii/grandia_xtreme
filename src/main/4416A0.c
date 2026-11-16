#include "common.h"
#include "main/n06.h"
#include "protos/main.h"


static inline void copyq(void *dst, void *src) {
    __asm__ volatile("lq $2, 0(%1)\n\tsq $2, 0(%0)" : : "r"(dst), "r"(src) : "$2", "memory");
}

s32 SndSePlay3D(s32 data, u8 grp, u8 flags, VuVec4 *vec) {
    s32 idx;
    SndSeSlotShifted *s;
    s32 n;

    grp &= 3;
    if (sndSeRequestEnabled == 0) {
        return -1;
    }
    idx = SndSeFindFreeSlot(grp);
    if (idx == -1) {
        return -1;
    }
    if (SndSeIsPlaying(grp, data) == 1) {
        return -2;
    }
    n = idx + 4;
    s = (SndSeSlotShifted *)((u32)(idx * sizeof(SndSeSlotShifted) + grp * sizeof(SndSeGroup)) + (u32)sndSeSlots);
    s->flags = flags;
    s->state = 1;
    s->wait = 0;
    s->handle = (grp << 6) + n;
    s->index = data;
    s->dist = 0;
    s->pan = 0;
    if (flags & 8) {
        SndSeSlot *it = &sndSeSlots[grp].u.s.slots[idx];
        copyq(it->pos, vec);
    }
    return idx;
}
