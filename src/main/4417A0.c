#include "common.h"
#include "main/n06.h"
#include "protos/main.h"


s32 SndSePlay(void *data, u8 grp, u8 flags, u8 arg3, u8 arg4) {
    s32 idx;
    SndSeSlotShifted *s;
    s32 n;

    if (sndSeRequestEnabled == 0) {
        return -1;
    }
    grp &= 3;
    idx = SndSeFindFreeSlot(grp);
    if (idx == -1) {
        return -1;
    }
    if (SndSeIsPlaying(grp, data) == 1) {
        return -2;
    }
    if (flags & 8) {
        flags ^= 8;
    }
    n = idx + 4;
    s = (SndSeSlotShifted *)((u32)(idx * sizeof(SndSeSlotShifted) + grp * sizeof(SndSeGroup)) + (u32)sndSeSlots);
    s->pan = arg4;
    s->state = 1;
    s->flags = flags;
    s->handle = (grp << 6) + n;
    s->index = (s32)data;
    s->dist = arg3;
    s->wait = 0;
    return idx;
}
